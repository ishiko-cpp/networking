/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_IPV4ADDRESS_HPP_
#define _ISHIKO_NETWORKING_IPV4ADDRESS_HPP_

#include <Ishiko/Errors.hpp>
#include <cstdint>
#include <string>

namespace Ishiko
{
namespace Networking
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
    std::string toString() const;

private:
    uint32_t m_address;
};

}
}

#endif
