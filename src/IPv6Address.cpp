// SPDX-FileCopyrightText: 2000-2025 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "IPv6Address.hpp"
#include "NetworkingErrorCategory.hpp"
#include <Ishiko/Text.hpp>
#include <ios>

using namespace Ishiko;

IPv6Address::IPv6Address()
{
    m_address.zero();
}

IPv6Address::IPv6Address(const Byte* bytes)
    : m_address(InplaceOctetBuffer<16>::From(bytes))
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
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

    m_address.bigEndianWordAt(0) = section1;
    m_address.bigEndianWordAt(2) = section2;
    m_address.bigEndianWordAt(4) = section3;
    m_address.bigEndianWordAt(6) = section4;
    m_address.bigEndianWordAt(8) = section5;
    m_address.bigEndianWordAt(10) = section6;
    m_address.bigEndianWordAt(12) = section7;
    m_address.bigEndianWordAt(14) = section8;
}

IPv6Address IPv6Address::Localhost()
{
    IPv6Address result;
    result.m_address.bigEndianWordAt(14) = 1;
    return result;
}

IPv6Address IPv6Address::Unspecified()
{
    // The default constructor creates the unspecified address already
    IPv6Address result;
    return result;
}

const InplaceOctetBuffer<16>& IPv6Address::value() const
{
    return m_address;
}

bool IPv6Address::operator==(IPv6Address other) const
{
    return (m_address == other.m_address);
}

bool IPv6Address::operator!=(IPv6Address other) const
{
    return (m_address != other.m_address);
}

std::string IPv6Address::toString() const
{
    std::string result;
    result += ASCII::ToHexString(m_address.bigEndianWordAt(0).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(2).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(4).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(6).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(8).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(10).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(12).toUint16());
    result += ':';
    result += ASCII::ToHexString(m_address.bigEndianWordAt(14).toUint16());
    return result;
}
