// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "tcp_server.h"
#include "protocol_message.h"

namespace fts {
namespace tcp {


Server::Server(asio::io_service & service)
    : _ioService(service)
    , _acceptor(_ioService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 0))
    , _socket(_ioService)
{
    accept();
}

Server::~Server() {
    _socket.close();
}

unsigned short Server::openPort() const {
    return _acceptor.local_endpoint().port();
}

std::vector<std::unique_ptr<ProtocolMessage>> Server::messages() {
    std::lock_guard<std::mutex> guard(_connectionsMutex);

    std::vector<std::unique_ptr<ProtocolMessage>> messages;
    for (auto && conn : _connections)
        while (auto msg = conn->read())
            messages.push_back(std::move(msg));

    return messages;
}

void Server::send(fts::ProtocolMessage && msg) {

    std::shared_ptr<fts::ProtocolMessage> shared_msg = std::make_shared<fts::ProtocolMessage>(std::move(msg));
    for (auto && conn : _connections)
        conn->send(shared_msg);
}

void Server::accept() {
    _acceptor.async_accept(_socket, [this](std::error_code ec) {
        if (!ec) {
            ServerConnection * raw = new ServerConnection(_ioService, std::move(_socket));
            std::unique_ptr<ServerConnection> unique(raw);

            std::lock_guard<std::mutex> guard(_connectionsMutex);
            _connections.push_back(std::move(unique));
        }

        accept();
    });
}


ServerConnection::ServerConnection(asio::io_service & io_service, asio::ip::tcp::socket && socket)
    : _ioService(io_service)
    , _socket(std::move(socket)) 
{ 
    receive();
}

ServerConnection::~ServerConnection() {
    _socket.close();
}

std::unique_ptr<fts::ProtocolMessage> ServerConnection::read() {
    if (_inMessages.empty())
        return nullptr;

    auto msg = std::move(_inMessages.front());
    _inMessages.pop_front();
    return msg;
}

void ServerConnection::send(std::shared_ptr<fts::ProtocolMessage> msg) {
    _ioService.post([this, msg]() {
        bool in_progress = !_outMessages.empty();
        _outMessages.push_back(msg);
        if (!in_progress)
            writeNext();
    });
}

void ServerConnection::receive() {
    _socket.async_receive(asio::buffer(_receiveBuf.data(), _receiveBuf.size()),
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                fts::ProtocolMessage * raw = new fts::ProtocolMessage(_receiveBuf.data(), length);
                std::unique_ptr<fts::ProtocolMessage> unique(raw);
                _inMessages.push_back(std::move(unique));
                receive();
            }
    });
}

void ServerConnection::writeNext() {
    auto && msg = _outMessages.front();
    auto buf = asio::buffer(msg->data(), msg->size());

    asio::async_write(_socket, buf,
        [this](std::error_code ec, std::size_t /*bytes_written*/) {
            if (ec) {
                _socket.close();
                return;
            }

            _outMessages.pop_front();
            if (!_outMessages.empty())
                writeNext();
    });
}

} // namespace tcp
} // namespace fts
