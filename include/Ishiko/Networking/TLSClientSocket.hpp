/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_

#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class TLSClientSocket
{
public:
    TLSClientSocket(Error& error) noexcept;

    void connect(IPv4Address address, Port port, Error& error) noexcept;

private:
    TCPClientSocket m_socket;
};

}

#endif
