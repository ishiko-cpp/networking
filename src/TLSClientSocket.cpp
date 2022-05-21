/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocket.hpp"

using namespace Ishiko;

TLSClientSocket::BotanTLSCallbacks::BotanTLSCallbacks(TCPClientSocket& socket)
    : m_socket(socket)
{
}

void TLSClientSocket::BotanTLSCallbacks::tls_emit_data(const uint8_t data[], size_t size)
{
    // TODO: how do I handle errors
    Error error;
    m_socket.write((const char*)data, size, error);
}

void TLSClientSocket::BotanTLSCallbacks::tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size)
{
    // TODO: how do I pass this back synchronously to the application
}

void TLSClientSocket::BotanTLSCallbacks::tls_alert(Botan::TLS::Alert alert)
{
    // TODO: how do I handle all possible alerts. One of those is in fact "close"
}

bool TLSClientSocket::BotanTLSCallbacks::tls_session_established(const Botan::TLS::Session& session)
{
    // Do not cache the session
    // TODO: do I need to support this?
    return false;
}

TLSClientSocket::BotanCredentialsManager::BotanCredentialsManager()
{
    // TODO: is this adequate?
    // We use the system managed trusted CA list
    m_stores.push_back(new Botan::System_Certificate_Store);
}

std::vector<Botan::Certificate_Store*> TLSClientSocket::BotanCredentialsManager::trusted_certificate_authorities(
    const std::string& type, const std::string& context)
{
    return m_stores;
}

std::vector<Botan::X509_Certificate> TLSClientSocket::BotanCredentialsManager::cert_chain(
    const std::vector<std::string>& cert_key_types, const std::string& type, const std::string& context)
{
    // We don't use client authentication
    return std::vector<Botan::X509_Certificate>();
}

Botan::Private_Key* TLSClientSocket::BotanCredentialsManager::private_key_for(const Botan::X509_Certificate& cert,
    const std::string& type, const std::string& context)
{
    // We don't use client authentication
    return nullptr;
}

TLSClientSocket::TLSClientSocket(Error& error) noexcept
    : m_socket(error), m_botanTLSCallbacks(m_socket), m_sessionManager(m_rng)
{
}

void TLSClientSocket::connect(IPv4Address address, Port port, Error& error) noexcept
{
    m_socket.connect(address, port, error);

    // TODO: replace "needfulsoftware.com" with correct domain but how?
    // TODO: I want TLS v.1.3 but only available in Botan 3.0.0. which is not released yet
    // TODO: what of TLS 1.2 is not supported, can I safely downgrade?
    m_tlsClient.reset(new Botan::TLS::Client(m_botanTLSCallbacks, m_sessionManager, m_credentials, m_policy, m_rng,
        Botan::TLS::Server_Information("needfulsoftware.com", port.number()), Botan::TLS::Protocol_Version::TLS_V12));

    // When the Botan TLS client is active it means the handshake has finished and we can start sending data so we
    // consider the connection has now been established and we return to the caller.
    while (!m_tlsClient->is_closed() && !m_tlsClient->is_active())
    {
        // TODO: buffer size?
        char buffer[10 * 1024];
        // TODO: handle error
        int n = m_socket.read(buffer, sizeof(buffer), error);

        // TODO: is there not an issue that I may read data from the server past the handshake. So application data
        // might be called. This would only happen if the server pushes data. So I need to buffer everything received
        // by tls_record_received

        // TODO: this returns the amount of data needed to complete the next record. Do I need to do something with
        // this.
        size_t needed = m_tlsClient->received_data((const uint8_t*)buffer, n);
    }
}

int TLSClientSocket::read(char* buffer, int length, Error& error)
{
    // TODO: how much TLS encoded data do I need to read to get the amount requested by the client?
    // I'll never know I think. So I can only read into an internal buffer and return data from that.
    return 0;
}

void TLSClientSocket::write(const char* buffer, int length, Error& error)
{
}
