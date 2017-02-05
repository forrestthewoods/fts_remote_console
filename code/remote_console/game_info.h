// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_GAME_INFO_H
#define FTS_GAME_INFO_H

#include "console.h"

// internal libs
#include <net/tcp_client.h>

// lang
#include <functional>


namespace fts {

    // forward declaration
    struct GameInfoBroadcast;

    struct GameServerInfo {
        GameServerInfo(fts::GameInfoBroadcast const & gib);
        GameServerInfo(GameServerInfo const & other);
        GameServerInfo(GameServerInfo && other);
        GameServerInfo & operator= (GameServerInfo const & other);
        
        bool operator==(GameServerInfo const & other) const;

        std::string ipaddr;
        std::string hostname;
        int32_t port;
        int32_t processId;
    };

    struct GameConnection {
        const GameServerInfo gameInfo;
        Console console;
        fts::tcp::Client tcpClient;

        GameConnection(asio::io_service & service, fts::GameServerInfo const & info);
    };

    typedef std::vector<std::unique_ptr<fts::GameConnection>> GameConnections;

} // namespace fts

#endif // FTS_GAME_INFO_H
