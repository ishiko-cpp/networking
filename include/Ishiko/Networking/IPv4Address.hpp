// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef _ISHIKO_CPP_NETWORKING_IPV4ADDRESS_HPP_
#define _ISHIKO_CPP_NETWORKING_IPV4ADDRESS_HPP_

#include <Ishiko/Errors.hpp>
#include <cstdint>
#include <string>

namespace Ishiko
{

class IPv4Address
{
public:
    IPv4Address();
    IPv4Address(uint32_t address);
    IPv4Address(const std::string& address, Error& error);
    static IPv4Address Localhost();

    uint32_t value() const;

    bool operator==(IPv4Address other) const;
    bool operator!=(IPv4Address other) const;
    bool operator<(IPv4Address other) const;
    std::string toString() const;

private:
    uint32_t m_address;
};

}

#endif
