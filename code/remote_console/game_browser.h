// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_GAME_BROWSER_H
#define FTS_GAME_BROWSER_H

#include "console.h"
#include "game_info.h"

// internal libs
#include <net/tcp_client.h>

// thirdparty
#include <imgui.h>


namespace fts {

    class GameBrowser {
    public:
        void Draw(GameConnections const & connections, ImVec2 pos, ImVec2 size);
    };

} // namespace fts


#endif // FTS_GAME_BROWSER_H
