// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "TLSServerSocket.hpp"

using namespace Ishiko;

TLSServerSocket::TLSServerSocket(IPv4Address address, Port port, std::string keyPath, std::string certificatePath,
    Error& error) noexcept
    : m_socket(address, port, SocketOption::none, error), m_keyPath(std::move(keyPath)),
    m_certificatePath(std::move(certificatePath))
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
