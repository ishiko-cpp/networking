/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocket.hpp"
#include <botan/x509path.h>

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
    int i = 0;
    ++i;
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

void TLSClientSocket::BotanTLSCallbacks::tls_verify_cert_chain(const std::vector<Botan::X509_Certificate>& cert_chain,
    const std::vector<std::shared_ptr<const Botan::OCSP::Response>>& ocsp,
    const std::vector<Botan::Certificate_Store*>& trusted_roots, Botan::Usage_Type usage, const std::string& hostname,
    const Botan::TLS::Policy& policy)
{
    if (cert_chain.empty())
    {
        throw Botan::Invalid_Argument("Certificate chain was empty");
    }

    Botan::Path_Validation_Restrictions restrictions(policy.require_cert_revocation_info(),
        policy.minimum_signature_strength());
    auto ocsp_timeout = std::chrono::milliseconds(1000);
    Botan::Path_Validation_Result result =
        Botan::x509_path_validate(cert_chain, restrictions, trusted_roots, hostname, usage,
            std::chrono::system_clock::now(), ocsp_timeout, ocsp);
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

    // TODO: handle error
    int n = m_socket.read(buffer, length, error);
    size_t needed = m_tlsClient->received_data((const uint8_t*)buffer, n);

    return 0;
}

void TLSClientSocket::write(const char* buffer, int length, Error& error)
{
    // TODO: how do we handle errors?
    m_tlsClient->send((const uint8_t*)buffer, length);
}
