// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith

#ifndef FTS_PROTOCOL_MESSAGE_H
#define FTS_PROTOCOL_MESSAGE_H

// internal
#include <protocol/protocol_generated.h>

// lang
#include <memory>


namespace fts {

typedef std::vector<uint8_t> ProtocolMessageBuffer;

// Helper for reading data off the wire
// Wraps a Nanomsg buffer and provides clean accessors to flatbuffer data
class ProtocolMessage {
public:

    ProtocolMessage(uint8_t const * data, size_t size);
    ProtocolMessage(ProtocolMessage && msg);

    bool empty() const;
    fts::Data type() const;
    uint8_t const * data() const;
    size_t size() const;

    fts::LogEntry const * asLogEntry() const;
    fts::GameInfoBroadcast const * asGameInfoBroadcast() const;
    fts::GameServerLogInterval const * asGameServerLogInterval() const;


private:
    ProtocolMessage() /* = delete */;
    ProtocolMessage(ProtocolMessage const &) /* = delete */;

    ProtocolMessageBuffer _buffer;
    fts::Message const * _msg;
};

} //namespace fts

#endif // FTS_PROTOCOL_MESSAGE_H
