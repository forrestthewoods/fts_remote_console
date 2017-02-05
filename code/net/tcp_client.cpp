// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "tcp_client.h"
#include "protocol_message.h"

namespace fts {
namespace tcp {

Client::Client(asio::io_service & service)
    : _connected(false)
    , _ioService(service)
    , _resolver(_ioService)
    , _socket(_ioService)
{
}

Client::~Client() {
    _socket.close();
}

void Client::connect(std::string const & ipaddr, uint16_t port) {
    asio::ip::basic_resolver_query<asio::ip::tcp> query(ipaddr, std::to_string(port));
    auto endpoints = _resolver.resolve(query);
    asio::async_connect(_socket, endpoints,
        [this](std::error_code ec, asio::ip::tcp::resolver::iterator) {
        if (!ec) {
            _connected = true;
            receive();
        }
    });
}

void Client::disconnect() {
    if (!_connected)
        return;

    _socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
    _socket.close();
    _connected = false;
}

bool Client::connected() const {
    return _connected;
}

std::unique_ptr<fts::ProtocolMessage> Client::read() {
    if (_inMessages.empty())
        return nullptr;

    auto msg = std::move(_inMessages.front());
    _inMessages.pop_front();
    return msg;
}

void Client::send(fts::ProtocolMessage && msg) {
    // Because we can't std::move a unique_ptr in a lambda capture until C++17. Leaks if lambda doesn't run.
    std::shared_ptr<fts::ProtocolMessage> shared_msg = std::make_shared<fts::ProtocolMessage>(std::move(msg));

    _ioService.post([this, shared_msg]() {
        bool in_progress = !_outMessages.empty();

        _outMessages.push_back(shared_msg);

        if (!in_progress)
            writeNext();
    });
}

void Client::receive() {
    _socket.async_receive(asio::buffer(_receiveBuf.data(), _receiveBuf.size()),
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                fts::ProtocolMessage * raw = new fts::ProtocolMessage(_receiveBuf.data(), length);
                std::unique_ptr<fts::ProtocolMessage> unique(raw);
                _inMessages.push_back(std::move(unique));
                receive();
            }
            else {
                _connected = false;
            }
    });
}

void Client::writeNext() {
    auto && msg = _outMessages.front();
    auto && protoMsg = msg.get();
    
    auto buf = asio::buffer(protoMsg->data(), protoMsg->size());
    asio::async_write(_socket, buf, 
        [this](std::error_code ec, std::size_t /*bytes_written*/) {
            if (ec) {
                _socket.close();
                return;
            }

            _outMessages.pop_front();
            if (!_outMessages.empty())
                writeNext();
        }
    );
}


} // namespace tcp
} // namespace fts
