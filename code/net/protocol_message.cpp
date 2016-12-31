// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "protocol_message.h"

namespace fts {

// ------------------ ProtocolMessage
ProtocolMessage::ProtocolMessage(uint8_t const * data, size_t size)
    : _buffer(data, data + size)    // create a copy of data
    , _msg(_buffer.data() != nullptr ? fts::GetMessage(_buffer.data()) : nullptr)
{
}

ProtocolMessage::ProtocolMessage(ProtocolMessage && msg) 
    : _buffer(std::move(msg._buffer))
    , _msg(msg._msg)
{
}

bool ProtocolMessage::empty() const {
    return _msg == nullptr;
}

fts::Data ProtocolMessage::type() const {
    return !empty() ? _msg->data_type() : fts::Data::NONE;
}

uint8_t const * ProtocolMessage::data() const {
    return _buffer.data();
}

size_t ProtocolMessage::size() const {
    return _buffer.size();
}

fts::LogEntry const * ProtocolMessage::asLogEntry() const {
    return (type() == fts::Data::LogEntry) ? static_cast<const fts::LogEntry *>(_msg->data()) : nullptr;
}

fts::GameInfoBroadcast const * ProtocolMessage::asGameInfoBroadcast() const {
    return (type() == fts::Data::GameInfoBroadcast) ? static_cast<const fts::GameInfoBroadcast *>(_msg->data()) : nullptr;
}

fts::GameServerLogInterval const * ProtocolMessage::asGameServerLogInterval() const {
    return (type() == fts::Data::GameServerLogInterval) ? static_cast<const fts::GameServerLogInterval *>(_msg->data()) : nullptr;
}


} // namespace fts
