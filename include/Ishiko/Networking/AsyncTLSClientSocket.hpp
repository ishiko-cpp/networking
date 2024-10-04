// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP

#include "IPv4Address.hpp"
#include "Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
    class AsyncTLSClientSocket
    {
    public:
        AsyncTLSClientSocket(Error& error) noexcept;

        void connect(IPv4Address address, Port port, Error& error) noexcept;
    };
}

#endif
