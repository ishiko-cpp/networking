/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_WINDOWS_TCPCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_WINDOWS_TCPCLIENTSOCKET_HPP_

#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include <Ishiko/Errors.hpp>
#include <winsock2.h>

namespace Ishiko
{
namespace Networking
{

// TODO: forbid copy, allow moves
class TCPClientSocket
{
public:
    TCPClientSocket(Error& error);
    TCPClientSocket(SOCKET socket);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error);

    // TODO: should length be size_t, same for return value
    int read(char* buffer, int length, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int length, Error& error);

    // TODO: there is an issue here with the fact that SIGPIPE will be triggered if we try any operations after
    // receiving FIN. This means either it becomes a very annoying precondition or I need to check state before all
    // operations. I think read is the annoying once because if we don't set error but return 0 when no more data then
    // any subsequent operation would not know there is an error. This seems to suggest it should really be treated as
    // an error. And it also means some headache with exception version of functions. So we may have to keep track of
    // socket state inside the class.

    IPv4Address getLocalIPAddress(Error& error) const;
    Port getLocalPort(Error& error) const;
    IPv4Address getPeerIPAddress(Error& error) const;
    Port getPeerPort(Error& error) const;

private:
    SOCKET m_socket;
};

}
}

#endif
