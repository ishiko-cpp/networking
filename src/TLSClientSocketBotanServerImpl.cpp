/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocketBotanServerImpl.hpp"
#include <botan/certstor_system.h>
#include <botan/data_src.h>
#include <botan/pkcs8.h>
#include <vector>

using namespace Ishiko;

TLSClientSocketBotanServerImpl::TLSClientSocketBotanServerImpl(TCPClientSocket&& socket, const std::string& keyPath,
    const std::string& certificatePath, Error& error) noexcept
    : m_socket(std::move(socket)), m_botanTLSCallbacks(m_socket, m_buffer), m_sessionManager(m_rng),
    m_credentials(keyPath, certificatePath, m_rng),
    m_tlsServer(m_botanTLSCallbacks, m_sessionManager, m_credentials, m_policy, m_rng)
{
   // When the Botan TLS client is active it means the handshake has finished and we can start sending data so we
    // consider the connection has now been established and we return to the caller.
    while (!m_tlsServer.is_closed() && !m_tlsServer.is_active())
    {
        // TODO: buffer size?
        Buffer buffer(10 * 1024);
        // TODO: handle error
        int n = m_socket.read(buffer, buffer.capacity(), error);

        // TODO: is there not an issue that I may read data from the server past the handshake. So application data
        // might be called. This would only happen if the server pushes data. So I need to buffer everything received
        // by tls_record_received

        // TODO: this returns the amount of data needed to complete the next record. Do I need to do something with
        // this.
        size_t needed = m_tlsServer.received_data((const uint8_t*)buffer.data(), n);
    }
}

void TLSClientSocketBotanServerImpl::connect(IPv4Address address, Port port, const std::string& hostname,
    Error& error) noexcept
{
    // TODO: this should never be called if we have created the server-side end of a socket pair
    // TODO: assert
}

int TLSClientSocketBotanServerImpl::read(char* buffer, int length, Error& error)
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
    Buffer localBuffer(10 * 1024);
    // TODO: handle error
    int n = m_socket.read(localBuffer, localBuffer.capacity(), error);
    if (n == 0)
    {
        // TODO: socket was closed on the other end
        return 0;
    }

    size_t needed = m_tlsServer.received_data((const uint8_t*)localBuffer.data(), n);
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

    return 0;
}

void TLSClientSocketBotanServerImpl::write(const char* buffer, int length, Error& error)
{
    // TODO: how do we handle errors?
    m_tlsServer.send((const uint8_t*)buffer, length);
}

const TCPClientSocket& TLSClientSocketBotanServerImpl::socket() const noexcept
{
    return m_socket;
}

TLSClientSocketBotanServerImpl::BotanTLSCallbacks::BotanTLSCallbacks(TCPClientSocket& socket, std::string& buffer)
    : m_socket(socket), m_buffer(buffer)
{
}

void TLSClientSocketBotanServerImpl::BotanTLSCallbacks::tls_emit_data(const uint8_t data[], size_t size)
{
    // TODO: how do I handle errors
    Error error;
    m_socket.write((const char*)data, size, error);
}

void TLSClientSocketBotanServerImpl::BotanTLSCallbacks::tls_record_received(uint64_t seq_no, const uint8_t data[],
    size_t size)
{
    m_buffer.append((const char*)data, size);
}

void TLSClientSocketBotanServerImpl::BotanTLSCallbacks::tls_alert(Botan::TLS::Alert alert)
{
    // TODO: how do I handle all possible alerts. One of those is in fact "close"
}

bool TLSClientSocketBotanServerImpl::BotanTLSCallbacks::tls_session_established(const Botan::TLS::Session& session)
{
    // Do not cache the session
    // TODO: do I need to support this?
    return false;
}

TLSClientSocketBotanServerImpl::BotanCredentialsManager::BotanCredentialsManager(const std::string& keyPath,
    const std::string& certificatePath, Botan::AutoSeeded_RNG& rng)
    : m_certificatePath(certificatePath)
{
    m_key.reset(Botan::PKCS8::load_key(keyPath, rng));
}

std::vector<Botan::Certificate_Store*>
TLSClientSocketBotanServerImpl::BotanCredentialsManager::trusted_certificate_authorities(const std::string& type,
    const std::string& context)
{
    // TODO: we don't do client authentication yet
    return std::vector<Botan::Certificate_Store*>();
}

std::vector<Botan::X509_Certificate> TLSClientSocketBotanServerImpl::BotanCredentialsManager::cert_chain(
    const std::vector<std::string>& cert_key_types, const std::string& type, const std::string& context)
{
    return { Botan::X509_Certificate(m_certificatePath) };
}

Botan::Private_Key* TLSClientSocketBotanServerImpl::BotanCredentialsManager::private_key_for(
    const Botan::X509_Certificate& cert, const std::string& type, const std::string& context)
{
    return m_key.get();
}
