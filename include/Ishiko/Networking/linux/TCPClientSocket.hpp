/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_

#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
namespace Networking
{

// TODO: forbid copy, allow moves
class TCPClientSocket
{
public:
    TCPClientSocket(Error& error);
    TCPClientSocket(int socket);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error);

    // TODO: should length be size_t, same for return value
    int read(char* buffer, int length, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int length, Error& error);

    IPv4Address getLocalIPAddress(Error& error) const;
    Port getLocalPort(Error& error) const;
    IPv4Address getPeerIPAddress(Error& error) const;
    Port getPeerPort(Error& error) const;

private:
    int m_socket;
};

}
}

#endif
