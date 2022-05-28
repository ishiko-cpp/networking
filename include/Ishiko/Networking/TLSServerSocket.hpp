/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP_

#include "TCPServerSocket.hpp"
#include "TLSClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <string>

namespace Ishiko
{

class TLSServerSocket
{
public:
    TLSServerSocket(IPv4Address address, Port port, std::string keyPath, std::string certificatePath,
        Error& error) noexcept;

    TLSClientSocket accept(Error& error) noexcept;

    IPv4Address ipAddress() const;
    Port port() const;

private:
    TCPServerSocket m_socket;
    std::string m_keyPath;
    std::string m_certificatePath;
};

}

#endif
