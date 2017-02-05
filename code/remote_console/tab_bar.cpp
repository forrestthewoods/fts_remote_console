// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "tab_bar.h"

namespace fts {

TabBar::TabBar() 
    : _focus(nullptr)
{
}

void TabBar::Draw(fts::GameConnections & connections, ImVec2 pos, ImVec2 size) {
    const float TAB_BTN_WIDTH = 90;
    const float TAB_BTN_HEIGHT = 20;

    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(pos, ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiSetCond_Always);

    ImGui::Begin("tab_bar", NULL, window_flags);

        if (ImGui::Button("Game Browser", ImVec2(TAB_BTN_WIDTH, TAB_BTN_HEIGHT)))
            _focus = nullptr;

        for (auto && connection : connections) {
            if (connection->tcpClient.connected()) {
                int len = std::sprintf(_buf.data(), "%s:%d", connection->gameInfo.hostname.c_str(), connection->gameInfo.processId);
                
                ImGui::SameLine();
                if (ImGui::Button(_buf.data(), ImVec2((float)(10 + 8*len), TAB_BTN_HEIGHT)))
                    _focus = connection.get();
            }
        }

        ImGui::Separator();
    ImGui::End();
}

fts::GameConnection * TabBar::focus() const {
    return _focus;
}

} // namespace fts
