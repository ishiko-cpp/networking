/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddress.hpp"

using namespace Ishiko;

IPAddress::IPAddress()
{
}

IPAddress::IPAddress(IPv4Address address)
    : m_address(std::move(address))
{
}

IPAddress::IPAddress(IPv6Address address)
    : m_address(std::move(address))
{
}

bool IPAddress::isIPv4() const noexcept
{
    return (m_address.which() == 0);
}

bool IPAddress::isIPv6() const noexcept
{
    return (m_address.which() == 1);
}
