// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_TCPCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_TCPCLIENTSOCKET_HPP

#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include "../SocketOption.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>
#define NOMINMAX // TODO: what rubbish, how do I put this ugly windows logic somewhere central
#include <winsock2.h>

namespace Ishiko
{

class TCPClientSocket
{
public:
    TCPClientSocket(int socket_options, Error& error) noexcept;
    TCPClientSocket(SOCKET socket) noexcept;
    TCPClientSocket(const TCPClientSocket& other) = delete;
    TCPClientSocket(TCPClientSocket&& other);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error) noexcept;

    // TODO: should return value be size_t
    int read(ByteBuffer& buffer, size_t count, Error& error);
    int read(ByteBuffer& buffer, size_t offset, size_t count, Error& error);
    // TODO: should length be size_t, same for return value
    int read(char* buffer, int count, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int count, Error& error);

    void shutdown(Error& error);
    void close();

    IPv4Address getLocalIPAddress(Error& error) const;
    Port getLocalPort(Error& error) const;
    IPv4Address getPeerIPAddress(Error& error) const;
    Port getPeerPort(Error& error) const;

public:
    // TODO: back to private
    SOCKET m_socket;
};

}

#endif
