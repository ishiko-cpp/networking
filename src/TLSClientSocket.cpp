/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocket.hpp"
#include "TLSClientSocketBotanClientImpl.hpp"
#include "TLSClientSocketBotanServerImpl.hpp"
#include <Ishiko/Memory.hpp>

using namespace Ishiko;

TLSClientSocket::TLSClientSocket(Error& error) noexcept
// TODO: can't allocate memory here
    : m_impl(new TLSClientSocketBotanClientImpl(error))
{
}

TLSClientSocket::TLSClientSocket(TCPClientSocket&& socket, const std::string& keyPath,
    const std::string& certificatePath, Error& error) noexcept
// TODO: can't allocate memory here
    : m_impl(new TLSClientSocketBotanServerImpl(std::move(socket), keyPath, certificatePath, error))
{
}

void TLSClientSocket::connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept
{
    m_impl->connect(address, port, hostname, error);
}

int TLSClientSocket::read(char* buffer, int length, Error& error)
{
    return m_impl->read(buffer, length, error);
}

void TLSClientSocket::write(const char* buffer, int length, Error& error)
{
    m_impl->write(buffer, length, error);
}

IPv4Address TLSClientSocket::getLocalIPAddress(Error& error) const
{
    return m_impl->socket().getLocalIPAddress(error);
}

Port TLSClientSocket::getLocalPort(Error& error) const
{
    return m_impl->socket().getLocalPort(error);
}

IPv4Address TLSClientSocket::getPeerIPAddress(Error& error) const
{
    return m_impl->socket().getPeerIPAddress(error);
}

Port TLSClientSocket::getPeerPort(Error& error) const
{
    return m_impl->socket().getPeerPort(error);
}
