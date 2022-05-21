/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_

#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include <botan/auto_rng.h>
#include <botan/tls_callbacks.h>
#include <botan/tls_session_manager.h>
#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class TLSClientSocket
{
public:
    TLSClientSocket(Error& error) noexcept;

    void connect(IPv4Address address, Port port, Error& error) noexcept;

    // TODO: should length be size_t, same for return value
    int read(char* buffer, int length, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int length, Error& error);

private:
    class BotanTLSCallbacks : public Botan::TLS::Callbacks
    {
    public:
        BotanTLSCallbacks(TCPClientSocket& socket);

        void tls_emit_data(const uint8_t data[], size_t size) override;
        void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override;
        void tls_alert(Botan::TLS::Alert alert) override;
        bool tls_session_established(const Botan::TLS::Session& session) override;

    private:
        TCPClientSocket& m_socket;
    };

    TCPClientSocket m_socket;
    BotanTLSCallbacks m_botanTLSCallbacks;
    // TODO: what is the autoseed RNG, is it safe enough
    Botan::AutoSeeded_RNG m_rng;
    // TODO: do I need this although I said the session is never cached above?
    Botan::TLS::Session_Manager_In_Memory m_sessionManager;
};

}

#endif
