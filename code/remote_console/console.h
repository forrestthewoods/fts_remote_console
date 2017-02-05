// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_CONSOLE_H
#define FTS_CONSOLE_H


#include "imgui_ext.h"

// internal libs
#include <net/protocol_message.h>
#include <flatbuffers/flatbuffers.h>

// thirdparty
#include <imgui.h>

// lang
#include <array>
#include <vector>
#include <string>
#include <unordered_map>


// forward declarations
namespace fts {
    struct GameConnection;
}

namespace fts {

class ConsoleEntry {
public:
    ConsoleEntry(ProtocolMessage && protoMsg);
    ConsoleEntry(ConsoleEntry && other);

    const char * time() const;
    const char * category() const;
    const char * msg() const;

    size_t categoryHash() const;

private:
    ProtocolMessage _protoMsg;
    fts::LogEntry const * _logEntry;
    size_t _category;
    std::array<char, 16> _timeStr;
};

class Console
{
public:

    Console();
    void Clear();        
    void AddLog(ProtocolMessage && protoMsg);
    void Draw(GameConnection & connection, ImVec2 pos, ImVec2 size);

private:

    std::unordered_map<size_t, std::pair<std::string, bool>> _categories;   // bool = enabled
    std::vector<std::string> _categoriesSorted;

    std::vector<ConsoleEntry>  _entries;

    flatbuffers::FlatBufferBuilder fbb;

    ImGui::ImGuiTextFuzzyFilter _fuzzyFilter;
    bool                _scrollToBottom;
    bool                _wrap;
    bool                _firstDraw;
    bool                _logToClipboard;
    float               _logInterval;
};


} // namespace fts

#endif // fts_console_h