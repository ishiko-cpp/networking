/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/TCPServerSocket.hpp"
#include "ErrorCategory.hpp"

using namespace std;

namespace Ishiko
{
namespace Networking
{

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error)
    : m_address(address), m_port(port)
{
    // TODO: the address and port may not be what was passed in! Port could be 0 and so could be address, 
    // we need to get them after the bind

    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    winsockAddress.sin_addr.s_addr = htonl(address.value());
    winsockAddress.sin_port = htons(port.number());
    
    int err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;\

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    m_address = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, SOMAXCONN);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

TCPServerSocket::~TCPServerSocket()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
    }
}

TCPClientSocket TCPServerSocket::accept(Error& error)
{
    SOCKET clientSocket = WSAAccept(m_socket, NULL, NULL, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    return TCPClientSocket(clientSocket);
}

IPv4Address TCPServerSocket::address() const
{
    return m_address;
}

Port TCPServerSocket::port() const
{
    return m_port;
}

}
}
