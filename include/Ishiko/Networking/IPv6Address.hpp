/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP_
#define _ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP_

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

    const FixedBuffer<16>& value() const;

    bool operator==(IPv6Address other) const;
    bool operator!=(IPv6Address other) const;
    std::string toString() const;

private:
    FixedBuffer<16> m_address;
};

}

#endif
