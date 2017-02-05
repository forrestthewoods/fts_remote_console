// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "game_info.h"

// internal libs
#include <protocol/protocol_generated.h>


namespace fts {

    GameServerInfo::GameServerInfo(fts::GameInfoBroadcast const & gib)
        : ipaddr(gib.ipaddr()->str())
        , hostname(gib.hostname()->str())
        , port(gib.port())
        , processId(gib.processid())
    {
    }

    GameServerInfo::GameServerInfo(GameServerInfo const & other) 
        : ipaddr(other.ipaddr)
        , hostname(other.hostname)
        , port(other.port)
        , processId(other.processId)
    {
    }
    
    GameServerInfo::GameServerInfo(GameServerInfo && other) 
        : ipaddr(std::move(other.ipaddr))
        , hostname(std::move(other.hostname))
        , port(other.port)
        , processId(other.processId)
    {
    }

    bool GameServerInfo::operator==(GameServerInfo const & other) const {
        return ipaddr == other.ipaddr && hostname == other.hostname && port == other.port && processId == other.processId;
    }

    GameConnection::GameConnection(asio::io_service & service, fts::GameServerInfo const & info)
        : gameInfo(info)
        , tcpClient(service)
    { 
    }

} // namespace fts
