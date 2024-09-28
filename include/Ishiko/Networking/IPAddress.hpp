// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_IPADDRESS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_IPADDRESS_HPP

#include "IPv4Address.hpp"
#include "IPv6Address.hpp"
#include <boost/variant.hpp>

namespace Ishiko
{
    class IPAddress
    {
    public:
        IPAddress();
        IPAddress(IPv4Address address);
        IPAddress(IPv6Address address);

        bool isIPv4() const noexcept;
        bool isIPv6() const noexcept;

        const IPv4Address& asIPv4Address() const;
        const IPv6Address& asIPv6Address() const;

    private:
        boost::variant<IPv4Address, IPv6Address> m_address;
    };
}

#endif
