// SPDX-FileCopyrightText: 2000-2025 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP

#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>
#include <string>
#include <vector>

namespace Ishiko
{
    class IPv6Address
    {
    public:
        IPv6Address();
        IPv6Address(const Byte* bytes);
        IPv6Address(const std::string& address, Error& error);
        static IPv6Address Localhost();
        static IPv6Address Unspecified();

        const InplaceOctetBuffer<16>& value() const;

        bool operator==(IPv6Address other) const;
        bool operator!=(IPv6Address other) const;
        std::string toString() const;

    private:
        InplaceOctetBuffer<16> m_address;
    };
}

#endif
