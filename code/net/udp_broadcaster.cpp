// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "udp_broadcaster.h"
#include "protocol_message.h"

namespace fts {
namespace udp {


Broadcaster::Broadcaster(asio::io_service & service, uint16_t port)
    : _ioService(service)
    , _socket(_ioService)
    , _endpoint(asio::ip::address_v4::broadcast(), port)
{
    _socket.open(asio::ip::udp::v4());
    _socket.set_option(asio::socket_base::broadcast(true));
}

Broadcaster::~Broadcaster() {
    _socket.close();
}

void Broadcaster::send(fts::ProtocolMessage && msg) {

    // Because we can't std::move in a lambda capture until C++17
    std::unique_ptr<fts::ProtocolMessage> unique = std::make_unique<fts::ProtocolMessage>(std::move(msg));
    fts::ProtocolMessage * raw = unique.release(); // raw pointer will leak if lambda is never run

    _ioService.post([this, raw]() {
        bool in_progress = !_outMessages.empty();

        std::unique_ptr<fts::ProtocolMessage> unique(raw);
        _outMessages.push_back(std::move(unique));

        if (!in_progress)
            sendNext();
    });
}

void Broadcaster::sendNext() {
    auto && msg = _outMessages.front();
    auto buf = asio::buffer(msg->data(), msg->size());

    _socket.async_send_to(buf, _endpoint,
        [this](std::error_code ec, std::size_t bytes_written) {
        if (ec) {
            _socket.close();
            return;
        }

        _outMessages.pop_front();
        if (!_outMessages.empty())
            sendNext();
    });
}

} // namespace udp
} // namespace fts
