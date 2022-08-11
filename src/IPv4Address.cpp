/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv4Address.hpp"
#include "NetworkingErrorCategory.hpp"
#include <Ishiko/Text.hpp>

using namespace Ishiko;

IPv4Address::IPv4Address()
    : m_address(0)
{
}

IPv4Address::IPv4Address(uint32_t address)
    : m_address(address)
{
}

IPv4Address::IPv4Address(const std::string& address, Error& error)
{
    std::string::const_iterator it = address.begin();
    while ((it != address.end()) && (*it != '.'))
    {
        ++it;
    }
    if (*it != '.')
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }
    unsigned char byte1 = 0;
    ASCII::Convert(address.cbegin(), it, byte1, error);
    if (error)
    {
        return;
    }

    std::string::const_iterator startIt = ++it;
    while ((it != address.end()) && (*it != '.'))
    {
        ++it;
    }
    if (*it != '.')
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }
    unsigned char byte2 = 0;
    ASCII::Convert(startIt, it, byte2, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while ((it != address.end()) && (*it != '.'))
    {
        ++it;
    }
    if (*it != '.')
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }
    unsigned char byte3 = 0;
    ASCII::Convert(startIt, it, byte3, error);
    if (error)
    {
        return;
    }

    startIt = ++it;
    while (it != address.end())
    {
        ++it;
    }
    unsigned char byte4 = 0;
    ASCII::Convert(startIt, it, byte4, error);
    if (error)
    {
        return;
    }

    m_address = ((byte1 << 24) + (byte2 << 16) + (byte3 << 8) + byte4);
}

IPv4Address IPv4Address::Localhost()
{
    IPv4Address result;
    result.m_address = ((127 << 24) + 1);
    return result;
}

uint32_t IPv4Address::value() const
{
    return m_address;
}

bool IPv4Address::operator==(IPv4Address other) const
{
    return (m_address == other.m_address);
}

bool IPv4Address::operator!=(IPv4Address other) const
{
    return (m_address != other.m_address);
}

std::string IPv4Address::toString() const
{
    std::string result;
    result += std::to_string(m_address >> 24);
    result += '.';
    result += std::to_string((m_address >> 16) & 0xFF);
    result += '.';
    result += std::to_string((m_address >> 8) & 0xFF);
    result += '.';
    result += std::to_string(m_address & 0xFF);
    return result;
}
