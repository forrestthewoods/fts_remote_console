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
#include "imgui_ext.h"

// internal libs
#include <net/protocol_message.h>

// lang
#include <stdio.h>


namespace fts {


ConsoleEntry::ConsoleEntry(ProtocolMessage && protoMsg)
    : _protoMsg(std::move(protoMsg))
    , _logEntry(_protoMsg.asLogEntry())
    , _category(std::hash<std::string>()(_logEntry->category()->c_str()))
{
    float milliseconds = _logEntry->time();
    float seconds = milliseconds / 1000;
    std::sprintf(_timeStr.data(), "%.2f", seconds);
}

ConsoleEntry::ConsoleEntry(ConsoleEntry && other) 
    : _protoMsg(std::move(other._protoMsg))
    , _logEntry(other._logEntry)
    , _category(other._category)
    , _timeStr(other._timeStr)
{
}
const char * ConsoleEntry::time() const {
    return _timeStr.data();
}

size_t ConsoleEntry::categoryHash() const {
    return _category;
}

const char * ConsoleEntry::category() const {
    return _logEntry->category()->c_str();
}

const char * ConsoleEntry::msg() const {
    return _logEntry->message()->c_str();
}



Console::Console()
    : _scrollToBottom(false)
    , _wrap(false)
    , _firstDraw(true)
    , _logToClipboard(false)
    , _logInterval(.5f)
{
}

void Console::Clear() {
    _fuzzyFilter.Clear();
    _entries.clear();
}

void Console::AddLog(ProtocolMessage && protoMsg) {

    ConsoleEntry entry(std::move(protoMsg));

    size_t hash = entry.categoryHash();
    if (_categories.find(hash) == _categories.end()) {
        _categories[hash] = std::make_pair(entry.category(), true);

        auto insert_iter = std::upper_bound(_categoriesSorted.begin(), _categoriesSorted.end(), entry.category());
        _categoriesSorted.insert(insert_iter, entry.category());
    }

    _entries.push_back(std::move(entry));

    size_t idx = _entries.size() - 1;
    _fuzzyFilter.Insert((void*)idx, _entries.back().msg());
    _scrollToBottom = true;
}


void Console::Draw(GameConnection & connection, ImVec2 pos, ImVec2 size)
{
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    float options_width = 200;


    // ------------ Output Area
    ImGui::SetNextWindowPos(pos, ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiSetCond_Always);
    ImGui::Begin("console", NULL, window_flags);

    ImGui::Columns(2);


    // ------------ Text Scroll
    ImGui::BeginChild("scrolling", ImVec2(0, -22 * ImGui::GetIO().FontGlobalScale), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (_logToClipboard)
            ImGui::LogToClipboard();

        // [TimeStamp] [Category] Message
        const char * entry_fmt = "[%s] [%s] %s";

        if (!_fuzzyFilter.Sorted)
            _fuzzyFilter.Sort();

        auto iter = _fuzzyFilter.Results.begin();
        while (iter != _fuzzyFilter.Results.end()) {
            ImGui::ImGuiTextFuzzyFilter::Entry const & fuzzy_entry = *iter;
            ++iter;

            if (!fuzzy_entry.valid())
                continue;

            size_t idx = (size_t)fuzzy_entry.data;
            ConsoleEntry & console_entry = _entries[idx];

            size_t entry_category = console_entry.categoryHash();
            bool enabled = _categories[entry_category].second;
            if (!enabled)
                continue;

            if (_wrap)
                ImGui::TextWrapped(entry_fmt, console_entry.time(), _categories[entry_category].first.c_str(), console_entry.msg());
            else
                ImGui::Text(entry_fmt, console_entry.time(), _categories[entry_category].first.c_str(), console_entry.msg());
        }

        if (_scrollToBottom)
            ImGui::SetScrollHere(1.0f);
        _scrollToBottom = false;

        if (_logToClipboard) {
            ImGui::LogFinish();
            _logToClipboard = false;
        }
    ImGui::EndChild(); 

    // ------------ Search Bar
    ImGui::ColumnSeparator();

    ImGui::AlignFirstTextHeightToWidgets();
        _fuzzyFilter.DrawSearchBar();

    // ------------ Options Area
    ImGui::NextColumn();
    if (_firstDraw) 
        ImGui::SetColumnOffset(-1, size.x - options_width);

    ImGui::BeginChild("Options");
    
        // ------------ Actions
        ImGui::Text("Actions");
        ImGui::Indent();
            if (ImGui::Button("Clear")) Clear();
            if (ImGui::Button("Copy")) _logToClipboard = true;
        ImGui::Unindent();


        // ------------ Options
        ImGui::ColumnSeparator();
        ImGui::Text("Options");
        ImGui::Indent();
            ImGui::Checkbox("Wrap", &_wrap);
        ImGui::Unindent();

        // ------------ Search Options
        ImGui::Text("Search Style");
        ImGui::Indent();
            _fuzzyFilter.DrawStyleRadios();
        ImGui::Unindent();

        // ------------ Categorties
        ImGui::ColumnSeparator();
        ImGui::Text("Categories");
        ImGui::Indent();
            auto string_hasher = std::hash<std::string>();
            for (auto && category : _categoriesSorted) {
                size_t hash = string_hasher(category);
                bool & enabled = _categories[hash].second;
                ImGui::Checkbox(category.c_str(), &enabled);
            }
        ImGui::Unindent();

        // ------------ Server Settings
        ImGui::ColumnSeparator();
        ImGui::Text("Server Settings");
        ImGui::Indent();
            ImGui::Text("Log: ");
            ImGui::SameLine();
            if (ImGui::SliderFloat("", &_logInterval, .1f, 3.f, "%1.1f")) {
                auto rate = fts::CreateGameServerLogInterval(fbb, _logInterval);
                auto fbbMsg = fts::CreateMessage(fbb, fts::Data::GameServerLogInterval, rate.o);
                fbb.Finish(fbbMsg);

                fts::ProtocolMessage protoMsg(fbb.GetBufferPointer(), fbb.GetSize());
                connection.tcpClient.send(std::move(protoMsg));
                fbb.Clear();
            }
        ImGui::Unindent();

        // ------------ Network
        ImGui::ColumnSeparator();
        ImGui::Text("Network");
        ImGui::Indent();
            if (ImGui::Button("Disconnect"))
                connection.tcpClient.disconnect();
        ImGui::Unindent();

    ImGui::EndChild();

    ImGui::End();

    _firstDraw = false;
}

} // namespace fts
