// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith

#ifndef FTS_UDP_BROADCAST_RECEIVER_H
#define FTS_UDP_BROADCAST_RECEIVER_H


// thirdparty
#include <asio/asio.hpp>

// lang
#include <array>
#include <deque>


// forward declarations
namespace fts {
    class ProtocolMessage;
}


namespace fts {
namespace udp {

class BroadcastReceiver {

public:
    BroadcastReceiver(asio::io_service & service, int port);
    ~BroadcastReceiver();

    std::unique_ptr<fts::ProtocolMessage> read();

private:

    void receive();

    asio::io_service & _ioService;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _endpoint;

    std::array<uint8_t, 4096> _receiveBuf;
    std::deque<std::unique_ptr<fts::ProtocolMessage>> _inMessages;
};


} // namespace udp
} // namespace fts


#endif // FTS_UDP_BROADCAST_RECEIVER_H