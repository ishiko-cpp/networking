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
    : m_address(move(address)), m_port(move(port)), m_socket(INVALID_SOCKET)
{
    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    int err = WSAHtons(m_socket, port.number(), &winsockAddress.sin_port);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    err = WSAHtonl(m_socket, address.value(), &winsockAddress.sin_addr.s_addr);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

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

}
}
