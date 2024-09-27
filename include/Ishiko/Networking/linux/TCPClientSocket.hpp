/*
    Copyright (c) 2022-2024 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef GUARD_ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP

#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include "../SocketOption.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>

namespace Ishiko
{

// TODO: forbid copy, allow moves
class TCPClientSocket
{
public:
    TCPClientSocket(Error& error) noexcept;
    TCPClientSocket(int socket) noexcept;
    TCPClientSocket(const TCPClientSocket& other) = delete;
    TCPClientSocket(TCPClientSocket&& other);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error);

    // TODO: should return value be size_t
    int read(ByteBuffer& buffer, size_t count, Error& error);
    int read(ByteBuffer& buffer, size_t offset, size_t count, Error& error);
    // TODO: should length be size_t, same for return value
    int read(char* buffer, int count, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int count, Error& error);

    // TODO: there is an issue here with the fact that SIGPIPE will be triggered if we try any operations after
    // receiving FIN. This means either it becomes a very annoying precondition or I need to check state before all
    // operations. I think read is the annoying once because if we don't set error but return 0 when no more data then
    // any subsequent operation would not know there is an error. This seems to suggest it should really be treated as
    // an error. And it also means some headache with exception version of functions. So we may have to keep track of
    // socket state inside the class.

    void shutdown(Error& error);
    void close();

    IPv4Address getLocalIPAddress(Error& error) const;
    Port getLocalPort(Error& error) const;
    IPv4Address getPeerIPAddress(Error& error) const;
    Port getPeerPort(Error& error) const;

private:
    int m_socket;
};

}

#endif
