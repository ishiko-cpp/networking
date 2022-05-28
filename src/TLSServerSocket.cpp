/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSServerSocket.hpp"

using namespace Ishiko;

TLSServerSocket::TLSServerSocket(IPv4Address address, Port port, Error& error) noexcept
    : m_socket(address, port, error)
{
}

TLSClientSocket TLSServerSocket::accept(Error& error) noexcept
{
    // TODO: handle errors
    TCPClientSocket clientSocket = m_socket.accept(error);
    return TLSClientSocket(std::move(clientSocket), error);
}

IPv4Address TLSServerSocket::ipAddress() const
{
    return m_socket.ipAddress();
}

Port TLSServerSocket::port() const
{
    return m_socket.port();
}
