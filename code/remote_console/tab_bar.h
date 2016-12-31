// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_TAB_BAR_H
#define FTS_TAB_BAR_H

#include "game_info.h"

// lang
#include <array>

// forward declarations
namespace fts {
    class Console;
}


namespace fts {

    class TabBar {
    public:

        TabBar();
        void Draw(fts::GameConnections & connections, ImVec2 pos, ImVec2 size);
        fts::GameConnection * focus() const;

    private:
        fts::GameConnection * _focus;
        std::array<char, 128> _buf;
    };

} // namespace fts

#endif // FTS_TAB_BAR_H
