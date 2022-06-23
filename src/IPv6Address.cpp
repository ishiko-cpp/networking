/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv6Address.hpp"
#include "NetworkingErrorCategory.hpp"
#include <Ishiko/Text.hpp>
#include <ios>

using namespace Ishiko;

IPv6Address::IPv6Address()
    : m_address(0)
{
}

IPv6Address::IPv6Address(const std::string& address, Error& error)
{
    // TODO: refactor this into a single loop

    const char separator = ':';
    std::string::const_iterator it = address.begin();
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section1 = 0;
    ASCII::Convert(address.cbegin(), it, std::ios::hex, section1, error);
    if (error)
    {
        return;
    }

    std::string::const_iterator startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section2 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section2, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section3 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section3, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section4 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section4, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section5 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section5, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section6 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section6, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != separator))
    {
        ++it;
    }
    if (*it != separator)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    uint16_t section7 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section7, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while (it != address.end())
    {
        ++it;
    }

    uint16_t section8 = 0;
    ASCII::Convert(startIt, it, std::ios::hex, section8, error);
    if (error)
    {
        return;
    }

    m_address = section1;
    m_address = (m_address << 16) + section2;
    m_address = (m_address << 16) + section3;
    m_address = (m_address << 16) + section4;
    m_address = (m_address << 16) + section5;
    m_address = (m_address << 16) + section6;
    m_address = (m_address << 16) + section7;
    m_address = (m_address << 16) + section8;
}

IPv6Address IPv6Address::Localhost()
{
    IPv6Address result;
    result.m_address = 1;
    return result;
}

boost::multiprecision::uint128_t IPv6Address::value() const
{
    return m_address;
}

std::string IPv6Address::toString() const
{
    std::string result;
    result += ASCII::ToHexString((uint16_t)((m_address >> 112) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 96) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 80) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 64) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 48) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 32) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)((m_address >> 16) & 0xFFFF));
    result += ':';
    result += ASCII::ToHexString((uint16_t)(m_address & 0xFFFF));
    return result;
}
