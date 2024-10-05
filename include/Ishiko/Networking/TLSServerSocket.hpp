// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TLSSERVERSOCKET_HPP

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
        TLSServerSocket(const TLSServerSocket& other) = delete;
        TLSServerSocket& operator=(const TLSServerSocket& other) = delete;
        // TODO: I probably want to allow the move constructor
        TLSServerSocket(TLSServerSocket&& other) noexcept = delete;
        TLSServerSocket& operator=(TLSServerSocket&& other) = delete;
        ~TLSServerSocket() = default;

        TLSClientSocket accept(Error& error) noexcept;

        IPAddress ipAddress() const;
        Port port() const;

    private:
        TCPServerSocket m_socket;
        std::string m_keyPath;
        std::string m_certificatePath;
    };
}

#endif
