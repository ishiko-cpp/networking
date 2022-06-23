/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_IPADDRESS_HPP_
#define _ISHIKO_CPP_NETWORKING_IPADDRESS_HPP_

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

private:
    boost::variant<IPv4Address, IPv6Address> m_address;
};

}

#endif
