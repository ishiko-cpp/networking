/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocket.hpp"

#include <botan/auto_rng.h>
#include <botan/certstor_system.h>
#include <botan/tls_callbacks.h>
#include <botan/tls_client.h>
#include <botan/tls_policy.h>
#include <botan/tls_session_manager.h>

using namespace Ishiko;

namespace
{

class BotanTLSCallbacks : public Botan::TLS::Callbacks
{
public:
    BotanTLSCallbacks(TCPClientSocket& socket)
        : m_socket(socket)
    {
    }

    void tls_emit_data(const uint8_t data[], size_t size) override
    {
        // TODO: how do I handle errors
        Error error;
        m_socket.write((const char*)data, size, error);
    }

    void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override
    {
        // TODO: how do I pass this back synchronously to the application
    }

    void tls_alert(Botan::TLS::Alert alert) override
    {
        // TODO: how do I handle all possible alerts. One of those is in fact "close"
    }

    bool tls_session_established(const Botan::TLS::Session& session) override
    {
        // Do not cache the session
        // TODO: do I need to support this?
        return false;
    }

    TCPClientSocket& m_socket;
};

// TODO: I need to put this somewhere else and make user configurable
class BotanCredentialsManager : public Botan::Credentials_Manager
{
public:
    BotanCredentialsManager()
    {
        // TODO: is this adequate?
        // We use the system managed trusted CA list
        m_stores.push_back(new Botan::System_Certificate_Store);
    }

    std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(const std::string& type,
        const std::string& context) override
    {
        return m_stores;
    }

    std::vector<Botan::X509_Certificate> cert_chain(const std::vector<std::string>& cert_key_types,
        const std::string& type, const std::string& context) override
    {
        // We don't use client authentication
        return std::vector<Botan::X509_Certificate>();
    }

    Botan::Private_Key* private_key_for(const Botan::X509_Certificate& cert, const std::string& type,
        const std::string& context) override
    {
        // We don't use client authentication
        return nullptr;
    }

private:
    std::vector<Botan::Certificate_Store*> m_stores;
};

}

TLSClientSocket::TLSClientSocket(Error& error) noexcept
    : m_socket(error)
{
}

void TLSClientSocket::connect(IPv4Address address, Port port, Error& error) noexcept
{
    m_socket.connect(address, port, error);

    BotanTLSCallbacks callbacks(m_socket);

    // TODO: what is the autoseed RNG, is it safe enough
    Botan::AutoSeeded_RNG rng;

    // TODO: do I need this although I said the session is never cached above?
    Botan::TLS::Session_Manager_In_Memory sessionManager(rng);
    
    BotanCredentialsManager credentials;
    
    Botan::TLS::Strict_Policy policy;
    
    // TODO: replace "needfulsoftware.com" with correct domain but how?
    // TODO: I want TLS v.1.3 but only available in Botan 3.0.0. which is not released yet
    // TODO: what of TLS 1.2 is not supported, can I safely downgrade?
    Botan::TLS::Client tlsClient(callbacks, sessionManager, credentials, policy, rng,
        Botan::TLS::Server_Information("needfulsoftware.com", port.number()),
        Botan::TLS::Protocol_Version::TLS_V12);
    // When the Botan TLS client is active it means the handshake has finished and we can start sending data so we
    // consider the connection has now been established and we return to the caller.
    while (!tlsClient.is_closed() && !tlsClient.is_active())
    {
        // TODO: buffer size?
        char buffer[10 * 1024];
        // TODO: handle error
        int n = m_socket.read(buffer, sizeof(buffer), error);

        tlsClient.received_data((const uint8_t*)buffer, n);
    }
}
