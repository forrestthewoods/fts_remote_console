// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


// internal libs
#include <protocol/protocol_generated.h>
#include <net/net_utils.h>
#include <net/tcp_server.h>
#include <net/udp_broadcaster.h>
#include <net/protocol_message.h>
#include <lib_fts/fts_timer.h>
#include <lib_fts/fts_sleep.h>

// thirdparty libs
#include <asio/asio.hpp>
#include <flatbuffers/flatbuffers.h>

// lang
#include <string>
#include <memory>
#include <array>
#include <stdlib.h>
#include <unordered_map>
#include <system_error>
#include <thread>


int main() {

    // Consts
    const int BROADCAST_PORT = 45326;

    // ASIO
    asio::io_service asio_service;
    fts::tcp::Server server(asio_service);
    fts::udp::Broadcaster broadcaster(asio_service, BROADCAST_PORT);

    std::thread t([&asio_service] { asio_service.run(); });
    t.detach();

    // Determine hostname
    std::array<char, 512> hostname;
    fts::GetHostname(hostname.data(), (int)hostname.size());

    // Determine IP address. I don't know the proper cross-platform way to enumerate adaptors. =[
    std::string local_ipaddr;
    {
        asio::ip::udp::resolver resolver(asio_service);
        asio::ip::udp::resolver::query query(asio::ip::udp::v4(), "google.com", "");
        asio::ip::udp::resolver::iterator endpoints = resolver.resolve(query);
        asio::ip::udp::endpoint ep = *endpoints;
        asio::ip::udp::socket socket(asio_service);
        socket.connect(ep);
        asio::ip::address addr = socket.local_endpoint().address();
        local_ipaddr = addr.to_string();
        socket.close();
    }

    // Log strings
    std::array<std::string, 7> log_types = { "system", "info", "warning", "error", "fatal", "notice", "log" };
    std::array<std::string, 7> log_messages = { "hello", "hola", "ciao", "bonjour", "guten tag", "ola", "namaste" };
    auto random_log = [&log_messages]() -> std::string const & { 
        return log_messages[rand() % log_messages.size()]; 
    };

    // Protocol helper
    flatbuffers::FlatBufferBuilder fbb;

    // Timers
    float log_interval = .5f;
    const float BROADCAST_INTERVAL = 5.f;
    fts::Stopwatch stopwatch;
    fts::Timer log_timer(log_interval);
    fts::Timer broadcast_timer(0.f); // send one immediately. then again every interval.

    // Main loop
    for (;;) {

        // Send a log message
        if (log_timer.Finished()) {
            auto type = fbb.CreateString(log_types[rand() % log_types.size()]);
            auto log_msg = random_log() + " " + random_log() + " " + random_log();
            auto logEntryMsg = fbb.CreateString(log_msg);
            auto entry = fts::CreateLogEntry(fbb, (float)stopwatch.elapsedMilliseconds(), type, logEntryMsg);
            auto fbbMsg = fts::CreateMessage(fbb, fts::Data::LogEntry, entry.o);
            fbb.Finish(fbbMsg);

            // Send log entry
            fts::ProtocolMessage protoMsg(fbb.GetBufferPointer(), fbb.GetSize());
            server.send(std::move(protoMsg));
            fbb.Clear();

            // Reset timer
            log_timer.Reset(log_interval);
        }

        // Broadcast server info
        if (broadcast_timer.Finished()) {

            auto ipaddr = fbb.CreateString(local_ipaddr);
            auto fbb_hostname = fbb.CreateString(std::string(hostname.data()));
            auto broadcast = fts::CreateGameInfoBroadcast(fbb, ipaddr, server.openPort(), fbb_hostname, fts::GetProcessId());
            auto fbbMsg = fts::CreateMessage(fbb, fts::Data::GameInfoBroadcast, broadcast.o);
            fbb.Finish(fbbMsg);

            // Send broadcast info
            fts::ProtocolMessage protoMsg(fbb.GetBufferPointer(), fbb.GetSize());
            broadcaster.send(std::move(protoMsg));
            fbb.Clear();

            // Reset timer
            broadcast_timer.Reset(BROADCAST_INTERVAL);
        }

        // Read messages from clients
        std::vector<std::unique_ptr<fts::ProtocolMessage>> messages = server.messages();
        for (auto && msg : messages) {
            if (auto interval_msg = msg->asGameServerLogInterval()) {
                log_interval = interval_msg->interval();
                if (log_interval < log_timer.RemainingSeconds())
                    log_timer.Reset(log_interval);
            }
        }
    }

    return 0;
}