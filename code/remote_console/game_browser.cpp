// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "game_browser.h"

namespace fts {

void GameBrowser::Draw(GameConnections const & connections, ImVec2 pos, ImVec2 size) {
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;


    // ------------ Output Area
    ImGui::SetNextWindowPos(pos, ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiSetCond_Always);
    ImGui::Begin("game_browser", NULL, window_flags);

    // ------------ Text Scroll
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false);

    for (auto && conn : connections) {
        GameServerInfo const & gi = conn->gameInfo;

        ImGui::Text("Host: %s", gi.hostname.c_str());
        ImGui::Indent(); 
            ImGui::Text("IP: %s:%d", gi.ipaddr.c_str(), gi.port);
            ImGui::Text("Pid: %d", gi.processId);
        ImGui::Unindent();

        ImGui::Spacing();

        bool connected = conn->tcpClient.connected();
        ImGui::Text("Status: %s", connected ? "connected" : "not connected");
        if (!connected) {
            if (ImGui::Button("Connect", ImVec2(90, 30)))
                conn->tcpClient.connect(gi.ipaddr, (uint16_t)gi.port);
        }
        
        ImGui::Separator();
    }

    ImGui::EndChild();

    ImGui::End();
}

} // namespace fts