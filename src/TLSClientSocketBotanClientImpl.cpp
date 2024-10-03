// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "TLSClientSocketBotanClientImpl.hpp"
#include "NetworkingErrorCategory.hpp"
#include <botan/x509path.h>
#include <iostream>

using namespace Ishiko;

TLSClientSocketBotanClientImpl::TLSClientSocketBotanClientImpl(int socket_options, Error& error) noexcept
    : m_socket{socket_options, error}, m_botanTLSCallbacks{m_socket, m_buffer}, m_sessionManager{m_rng}, m_port{0}
{
}

void TLSClientSocketBotanClientImpl::connect(IPv4Address address, Port port, const std::string& hostname,
    Error& error) noexcept
{
    m_socket.connect(address, port, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // Socket connection would block, a callback will need to be called to do the handshake once the socket is
            // connected
            m_port = port;
            m_hostname = hostname;
            m_state = State::waiting_for_connection;

            // TODO: I want TLS v.1.3 but only available in Botan 3.0.0. which is not released yet
    // TODO: what of TLS 1.2 is not supported, can I safely downgrade?
            m_tlsClient.reset(new Botan::TLS::Client(m_botanTLSCallbacks, m_sessionManager, m_credentials, m_policy, m_rng,
                Botan::TLS::Server_Information(hostname, port.number()), Botan::TLS::Protocol_Version::TLS_V12));

        }
        else
        {
            m_state = State::error;
        }
        return;
    }

    // TODO: I want TLS v.1.3 but only available in Botan 3.0.0. which is not released yet
    // TODO: what of TLS 1.2 is not supported, can I safely downgrade?
    m_tlsClient.reset(new Botan::TLS::Client(m_botanTLSCallbacks, m_sessionManager, m_credentials, m_policy, m_rng,
        Botan::TLS::Server_Information(hostname, port.number()), Botan::TLS::Protocol_Version::TLS_V12));


    // Connection successful, do the handshake
    doHandshake(port, hostname);
}

int TLSClientSocketBotanClientImpl::read(char* buffer, int length, Error& error)
{
    // TODO: how much TLS encoded data do I need to read to get the amount requested by the client?
    // I'll never know I think. So I can only read into an internal buffer and return data from that.

    // TODO: handle error
    if (!m_buffer.empty())
    {
        size_t count = m_buffer.size();
        if (m_buffer.size() > length)
        {
            count = length;
        }
        memcpy(buffer, m_buffer.c_str(), count);
        m_buffer.erase(m_buffer.begin(), (m_buffer.begin() + count));
        return count;
    }

    // TODO: buffer size?
    ByteBuffer localBuffer(10 * 1024);
    // TODO: handle error
    int n = m_socket.read(localBuffer, localBuffer.capacity(), error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            m_state = State::waiting_for_read;
            // TODO: should this return 0, that is usually used to indicate socket has been closed
            return 0;
        }
    }
    if (n == 0)
    {
        // TODO: socket was closed on the other end
        return 0;
    }

    size_t needed = m_tlsClient->received_data((const uint8_t*)localBuffer.data(), n);
    // TODO: take into account needed

    // TODO: is it possible here that I read so little I don't have a full TLS packet and m_buffer is still empty but
    // the socket is not closed.
    if (!m_buffer.empty())
    {
        size_t count = m_buffer.size();
        if (m_buffer.size() > length)
        {
            count = length;
        }
        memcpy(buffer, m_buffer.c_str(), count);
        m_buffer.erase(m_buffer.begin(), (m_buffer.begin() + count));
        return count;
    }

    // TODO: should this return 0, that is usually used to indicate socket has been closed
    return 0;
}

void TLSClientSocketBotanClientImpl::write(const char* buffer, int length, Error& error)
{
    // TODO: how do we handle errors?
    m_tlsClient->send((const uint8_t*)buffer, length);
}

const TCPClientSocket& TLSClientSocketBotanClientImpl::socket() const noexcept
{
    return m_socket;
}

TCPClientSocket& TLSClientSocketBotanClientImpl::socket() noexcept
{
    return m_socket;
}

bool TLSClientSocketBotanClientImpl::isConnected() const
{
    return m_tlsClient->is_active();
}

void TLSClientSocketBotanClientImpl::onCallback()
{
    // TODO: erro handling
    Error error;
    switch (m_state)
    {
    case State::waiting_for_connection:
        doHandshake(m_port, m_hostname);
        break;

    case State::waiting_for_read_during_handshake:
        doReadDuringHandshake(error);
        break;

    case State::waiting_for_write_during_handshake:
        // TODO
        break;

    case State::waiting_for_read:
        // TODO: I think I don't need to do anything here
        break;

    case State::error:
        // TODO
        break;
    }
}

void TLSClientSocketBotanClientImpl::doHandshake(Port port, const std::string& hostname)
{
    // TODO: handle errors
    Error todo_error;

    // When the Botan TLS client is active it means the handshake has finished and we can start sending data so we
    // consider the connection has now been established and we return to the caller.
    while (!m_tlsClient->is_closed() && !m_tlsClient->is_active())
    {
        // TODO: what if a write blocks during this loop?

        doReadDuringHandshake(todo_error);
        if (todo_error)
        {
            if (todo_error.code() == NetworkingErrorCategory::Value::would_block)
            {
                m_state = State::waiting_for_read_during_handshake;
            }
            else
            {
                // TODO
                m_state = State::error;
            }
            break;
        }
    }

    // TODO: need to change state here if we are closed or active
}

void TLSClientSocketBotanClientImpl::doReadDuringHandshake(Error& error)
{
    // TODO: handle errors

    while (!m_tlsClient->is_closed() && !m_tlsClient->is_active())
    {
        // TODO: what if a write blocks during this loop?

        // TODO: buffer size?
        ByteBuffer buffer(10 * 1024);

        // TODO: handle error
        int n = m_socket.read(buffer, buffer.capacity(), error);
        if (error)
        {
            if (error.code() == NetworkingErrorCategory::Value::would_block)
            {
                std::cerr << "TLSClientSocketBotanClientImpl::doReadDuringHandshake would block" << std::endl;
                m_state = State::waiting_for_read_during_handshake;
            }
            else
            {
                // TODO
                m_state = State::error;
            }
            break;
        }
        if (n == 0)
        {
            std::cerr << "Returning 0" << std::endl;
        }

        // TODO: how do I handle socket close here?

        // TODO: is there not an issue that I may read data from the server past the handshake. So application data
        // might be called. This would only happen if the server pushes data. So I need to buffer everything received
        // by tls_record_received

        // TODO: this returns the amount of data needed to complete the next record. Do I need to do something with
        // this.
        size_t needed = m_tlsClient->received_data((const uint8_t*)buffer.data(), n);
    }

    // TODO: need to change state here if we are closed or active
}

TLSClientSocketBotanClientImpl::BotanTLSCallbacks::BotanTLSCallbacks(TCPClientSocket& socket, std::string& buffer)
    : m_socket(socket), m_buffer(buffer)
{
}

void TLSClientSocketBotanClientImpl::BotanTLSCallbacks::tls_emit_data(const uint8_t data[], size_t size)
{
    // TODO: how do I handle errors
    Error error;
    m_socket.write((const char*)data, size, error);

    if (error)
    {
        // TODO
        std::cerr << "Error tls_emit_data" << std::endl;
    }

    // TODO: handle would_block error, unlikely to happen so I leave it for now
}

void TLSClientSocketBotanClientImpl::BotanTLSCallbacks::tls_record_received(uint64_t seq_no, const uint8_t data[],
    size_t size)
{
    m_buffer.append((const char*)data, size);
}

void TLSClientSocketBotanClientImpl::BotanTLSCallbacks::tls_alert(Botan::TLS::Alert alert)
{
    // TODO: how do I handle all possible alerts. One of those is in fact "close"
}

bool TLSClientSocketBotanClientImpl::BotanTLSCallbacks::tls_session_established(const Botan::TLS::Session& session)
{
    // Do not cache the session
    // TODO: do I need to support this?
    return false;
}

void TLSClientSocketBotanClientImpl::BotanTLSCallbacks::tls_verify_cert_chain(
    const std::vector<Botan::X509_Certificate>& cert_chain,
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

TLSClientSocketBotanClientImpl::BotanCredentialsManager::BotanCredentialsManager()
{
    // TODO: is this adequate?
    // We use the system managed trusted CA list
    m_stores.push_back(new Botan::System_Certificate_Store);
}

std::vector<Botan::Certificate_Store*>
TLSClientSocketBotanClientImpl::BotanCredentialsManager::trusted_certificate_authorities(const std::string& type,
    const std::string& context)
{
    return m_stores;
}

std::vector<Botan::X509_Certificate> TLSClientSocketBotanClientImpl::BotanCredentialsManager::cert_chain(
    const std::vector<std::string>& cert_key_types, const std::string& type, const std::string& context)
{
    // We don't use client authentication
    return std::vector<Botan::X509_Certificate>();
}

Botan::Private_Key* TLSClientSocketBotanClientImpl::BotanCredentialsManager::private_key_for(
    const Botan::X509_Certificate& cert, const std::string& type, const std::string& context)
{
    // We don't use client authentication
    return nullptr;
}
