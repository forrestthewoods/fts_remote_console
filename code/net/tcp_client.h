// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_TCP_CLIENT_H
#define FTS_TCP_CLIENT_H

// thirdparty
#include <asio/asio.hpp>

// lang
#include <string>
#include <deque>
#include <array>
#include <queue>


// forward declarations
namespace fts {
    class ProtocolMessage;
}

namespace fts {
namespace tcp {

class Client {
public:

    Client(asio::io_service & service);
    ~Client();

    void connect(std::string const & ipaddr, uint16_t port);
    void disconnect();

    bool connected() const;

    std::unique_ptr<fts::ProtocolMessage> read();
    void send(fts::ProtocolMessage && msg);

private:

    void receive();
    void writeNext();

    bool _connected;
    asio::io_service & _ioService;
    asio::ip::tcp::resolver _resolver;
    asio::ip::tcp::socket _socket;

    std::array<uint8_t, 4096> _receiveBuf;
    std::deque<std::unique_ptr<fts::ProtocolMessage>> _outMessages;
    std::deque<std::unique_ptr<fts::ProtocolMessage>> _inMessages;
};

} // namespace tcp
} // namespace fts

#endif // FTS_TCP_CLIENT_H