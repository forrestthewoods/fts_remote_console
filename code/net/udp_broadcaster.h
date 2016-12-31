// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith

#ifndef FTS_UDP_BROADCASTER_H
#define FTS_UDP_BROADCASTER_H


// thirdparty
#include <asio/asio.hpp>

// lang
#include <deque>

// forward declarations
namespace fts {
    class ProtocolMessage;
}


namespace fts {
namespace udp {

class Broadcaster {
public:
    Broadcaster(asio::io_service & service, uint16_t port);
    ~Broadcaster();

    void send(fts::ProtocolMessage && msg);

private:
    void sendNext();

    asio::io_service & _ioService;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _endpoint;
    std::deque<std::shared_ptr<fts::ProtocolMessage>> _outMessages;
};

} // namespace udp
} // namespace fts

#endif // FTS_UDP_BROADCASTER_H