/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP_
#define _ISHIKO_CPP_NETWORKING_IPV6ADDRESS_HPP_

#include <boost/multiprecision/cpp_int.hpp>
#include <Ishiko/Errors.hpp>
#include <string>

namespace Ishiko
{

class IPv6Address
{
public:
    IPv6Address();
    IPv6Address(const std::string& address, Error& error);
    static IPv6Address Localhost();

    boost::multiprecision::uint128_t value() const;

    std::string toString() const;

private:
    boost::multiprecision::uint128_t m_address;
};

}

#endif
