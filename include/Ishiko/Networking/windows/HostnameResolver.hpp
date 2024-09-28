// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_HOSTNAMERESOLVER_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_HOSTNAMERESOLVER_HPP

#include "../Hostname.hpp"
#include "../IPv4Address.hpp"
#include <Ishiko/Errors.hpp>
#include <string>
#include <vector>

namespace Ishiko
{
    class HostnameResolver
    {
    public:
        void resolve(const Hostname& name, std::vector<IPv4Address>& addresses, Error& error);
        void resolve(const std::string& name, std::vector<IPv4Address>& addresses, Error& error);
    };
}

#endif
