// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "udp_broadcast_receiver.h"
#include "protocol_message.h"


namespace fts {
namespace udp {

BroadcastReceiver::BroadcastReceiver(asio::io_service & service, int port)
    : _ioService(service)
    , _socket(service)
    , _endpoint(asio::ip::address_v4::any(), port)
{
    _socket.open(_endpoint.protocol());
    _socket.set_option(asio::ip::udp::socket::reuse_address(true));
    _socket.bind(_endpoint);

    receive();
}

BroadcastReceiver::~BroadcastReceiver() {
    _socket.close();
}

std::unique_ptr<fts::ProtocolMessage> BroadcastReceiver::read() {
    if (_inMessages.empty())
        return nullptr;

    auto msg = std::move(_inMessages.front());
    _inMessages.pop_front();
    return msg;
}

void BroadcastReceiver::receive() {
    _socket.async_receive_from(asio::buffer(_receiveBuf.data(), _receiveBuf.size()), _endpoint,
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                auto msg = std::make_unique<fts::ProtocolMessage>(_receiveBuf.data(), length);
                _inMessages.push_back(std::move(msg));
                receive();
            }
    });
}

/*
    receiver(boost::asio::io_service& io_service,
  const boost::asio::ip::address& listen_address)
: m_socket(io_service)
{

boost::asio::ip::address ipAddr = boost::asio::ip::address_v4::any();
boost::asio::ip::udp::endpoint listen_endpoint(
         ipAddr, 2368);

m_socket.open(listen_endpoint.protocol());
m_socket.bind(listen_endpoint);

m_socket.async_receive_from(
    boost::asio::buffer(m_data, max_length), m_sender_endpoint,
    boost::bind(&receiver::handle_receive_from, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void handle_receive_from(const boost::system::error_code& error,
  size_t bytes_recvd)
{
std::cout << "receive" << bytes_recvd << std::endl;


  m_socket.async_receive_from(
      boost::asio::buffer(m_data, max_length), m_sender_endpoint,
      boost::bind(&receiver::handle_receive_from, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}
*/

} // namespace udp
} // namespace fts