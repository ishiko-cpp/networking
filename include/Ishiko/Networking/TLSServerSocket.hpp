/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP_

#include "TCPServerSocket.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class TLSServerSocket
{
public:
    TLSServerSocket(IPv4Address address, Port port, Error& error) noexcept;

    IPv4Address ipAddress() const;
    Port port() const;

private:
    TCPServerSocket m_socket;
};

}

#endif
