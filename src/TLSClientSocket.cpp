/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocket.hpp"

using namespace Ishiko;

TLSClientSocket::TLSClientSocket(Error& error) noexcept
    : m_socket(error)
{
}

void TLSClientSocket::connect(IPv4Address address, Port port, Error& error) noexcept
{
    m_socket.connect(address, port, error);

    // TODO: TLS handshake
}
