// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_TCP_SERVER_H
#define FTS_TCP_SERVER_H

// thirdparty
#include <asio/asio.hpp>

// lang
#include <array>
#include <vector>
#include <deque>
#include <mutex>


// forward declarations
namespace fts {
    class ProtocolMessage;

    namespace tcp {
        class ServerConnection;
    }
}


namespace fts {
namespace tcp {


class Server {
public:
    Server(asio::io_service & service);
    ~Server();

    unsigned short openPort() const;
    std::vector<std::unique_ptr<ProtocolMessage>> messages();

    void send(fts::ProtocolMessage && msg);

private:
    void accept();

    asio::io_service & _ioService;
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::socket _socket;

    std::mutex _connectionsMutex;
    std::vector<std::unique_ptr<ServerConnection>> _connections;
};


class ServerConnection {
public:
    ServerConnection(asio::io_service & service, asio::ip::tcp::socket && socket);
    ~ServerConnection();

    std::unique_ptr<fts::ProtocolMessage> read();
    void send(std::shared_ptr<fts::ProtocolMessage> msg);

private:

    void receive();
    void writeNext();

    asio::io_service & _ioService;
    asio::ip::tcp::socket _socket;

    std::array<uint8_t, 4096> _receiveBuf;
    std::deque<std::shared_ptr<fts::ProtocolMessage>> _outMessages;
    std::deque<std::unique_ptr<fts::ProtocolMessage>> _inMessages;
};


} // namespace fts::tcp
} // namespace fts

#endif // FTS_TCP_CLIENT_H
