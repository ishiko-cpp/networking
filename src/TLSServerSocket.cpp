/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSServerSocket.hpp"

using namespace Ishiko;

TLSServerSocket::TLSServerSocket(IPv4Address address, Port port, std::string keyPath, std::string certificatePath,
    Error& error) noexcept
    : m_socket(address, port, error), m_keyPath(std::move(keyPath)), m_certificatePath(std::move(certificatePath))
{
}

TLSClientSocket TLSServerSocket::accept(Error& error) noexcept
{
    // TODO: handle errors
    TCPClientSocket clientSocket = m_socket.accept(error);
    return TLSClientSocket(std::move(clientSocket), m_keyPath, m_certificatePath, error);
}

IPAddress TLSServerSocket::ipAddress() const
{
    return m_socket.ipAddress();
}

Port TLSServerSocket::port() const
{
    return m_socket.port();
}
