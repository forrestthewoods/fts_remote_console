// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "console.h"
#include "game_info.h"
#include "game_browser.h"
#include "tab_bar.h"
#include "imgui_impl_glfw.h"
#include "high_dpi_handler.h"

// internal libs
#include <protocol/protocol_generated.h>
#include <net/tcp_client.h>
#include <net/udp_broadcast_receiver.h>
#include <lib_fts/fts_timer.h>

// thirdparty libs
#include <imgui.h>
#include <asio/asio.hpp>
#include <GLFW/glfw3.h>
#include <asio/asio.hpp>

// lang
#include <stdio.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <deque>



int main(int, char**) {

    // Consts
    const int BROADCAST_PORT = 45326;

	// Initialize 
	fts::InitDPI();

    // ASIO
    asio::io_service asio_service;
    fts::udp::BroadcastReceiver broadcast_receiver(asio_service, BROADCAST_PORT);
    std::thread t([&asio_service] { asio_service.run(); });
    t.detach();

    // Setup OpenGL window
    glfwSetErrorCallback([](int error, const char * description) { 
        fprintf(stderr, "Error %d: %s\n", error, description); 
    });
    if (!glfwInit())
        return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "fts_remote_console", NULL, NULL);
    glfwSetWindowSizeLimits(window, 300, 300, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);
    ImVec4 clear_color = ImColor(114, 144, 154);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.f;
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Data
    fts::TabBar tab_bar;
    fts::GameBrowser game_browser;
    fts::GameConnections game_connections;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // BeginFrame
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        // Pull TCP network messages for each game server connection
        for (auto && conn : game_connections) {
            while (auto msg = conn->tcpClient.read()) {
                if (auto log_entry = msg->asLogEntry())
                    conn->console.AddLog(std::move(*msg.release()));
            }
        }

        // Pull UDP broadcast messages telling us about available game servers
        while (auto msg = broadcast_receiver.read()) {
            if (auto broadcastMsg = msg->asGameInfoBroadcast()) {
                fts::GameServerInfo gi(*broadcastMsg);

                // If this is a new game server then create a new connection
                auto connection = std::find_if(game_connections.begin(), game_connections.end(), [&gi](std::unique_ptr<fts::GameConnection> const & conn) { return conn->gameInfo == gi; });
                if (connection == game_connections.end()) {
                    fts::GameConnection * raw = new fts::GameConnection(asio_service, gi);
                    std::unique_ptr<fts::GameConnection> unique(raw);
                    game_connections.push_back(std::move(unique));
                }
            }
        }

        // Draw TabBar
        const float TAB_BAR_HEIGHT = 40;
        tab_bar.Draw(game_connections, ImVec2(0, 0), ImVec2((float)display_w, TAB_BAR_HEIGHT));
        
        // Draw either the browser or the selected game console
        ImVec2 pos = ImVec2(0, TAB_BAR_HEIGHT);
        ImVec2 size = ImVec2((float)display_w, (float)display_h - TAB_BAR_HEIGHT);
        fts::GameConnection * focus = tab_bar.focus();
        if (focus)
            focus->console.Draw(*focus, pos, size);
        else
            game_browser.Draw(game_connections, pos, size);

        // OpenGL Rendering
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }


    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();

    return 0;
}
