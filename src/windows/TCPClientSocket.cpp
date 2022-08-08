/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/TCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

TCPClientSocket::TCPClientSocket(Error& error) noexcept
{
    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
}

TCPClientSocket::TCPClientSocket(SOCKET socket) noexcept
    : m_socket(socket)
{
}

TCPClientSocket::TCPClientSocket(TCPClientSocket&& other)
    : m_socket(other.m_socket)
{
    other.m_socket = INVALID_SOCKET;
}

TCPClientSocket::~TCPClientSocket()
{
    close();
}

void TCPClientSocket::connect(IPv4Address address, Port port, Error& error) noexcept
{
    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    winsockAddress.sin_port = htons(port.number());
    winsockAddress.sin_addr.s_addr = htonl(address.value());
    
    int err = WSAConnect(m_socket, reinterpret_cast<sockaddr*>(&winsockAddress), sizeof(winsockAddress), NULL, NULL,
        NULL, NULL);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
}

// TODO: test
int TCPClientSocket::read(Buffer& buffer, size_t count, Error& error)
{
    // TODO: bounds checks
    int err = recv(m_socket, reinterpret_cast<char*>(buffer.data()), count, 0);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
    return err;
}

// TODO: test
int TCPClientSocket::read(Buffer& buffer, size_t offset, size_t count, Error& error)
{
    // TODO: bounds checks
    int err = recv(m_socket, reinterpret_cast<char*>(buffer.data() + offset), count, 0);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
    return err;
}

int TCPClientSocket::read(char* buffer, int count, Error& error)
{
    int err = recv(m_socket, buffer, count, 0);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
    return err;
}

void TCPClientSocket::write(const char* buffer, int count, Error& error)
{
    int err = send(m_socket, buffer, count, 0);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
}

void TCPClientSocket::shutdown(Error& error)
{
    int err = ::shutdown(m_socket, SD_SEND);
    if (err == SOCKET_ERROR)
    {
        int lastError = WSAGetLastError();
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
}

void TCPClientSocket::close()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

IPv4Address TCPClientSocket::getLocalIPAddress(Error& error) const
{
    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    int err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }

    return IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
}

Port TCPClientSocket::getLocalPort(Error& error) const
{
    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    int err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }

    return Port(ntohs(boundAddress.sin_port));
}

IPv4Address TCPClientSocket::getPeerIPAddress(Error& error) const
{
    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    int err = getpeername(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }

    return IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
}

Port TCPClientSocket::getPeerPort(Error& error) const
{
    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    int err = getpeername(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }

    return Port(ntohs(boundAddress.sin_port));
}
