/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv4Address.hpp"
#include "ErrorCategory.hpp"
#include <Ishiko/Text.hpp>
#include <cstring>

using namespace Ishiko::Text;
using namespace std;

namespace Ishiko
{
namespace Networking
{
    
IPv4Address::IPv4Address()
{
    memset(m_address, 0, 4);
}

IPv4Address::IPv4Address(const string& address, Error& error)
{
    string::const_iterator it = address.begin();
    while ((it != address.end()) && (*it != '.'))
    {
        ++it;
    }
    if (*it != '.')
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
    unsigned char byte1 = 0;
    ASCII::Convert(address.cbegin(), it, byte1, error);
    if (error)
    {
        return;
    }

    string::const_iterator startIt = ++it;
    while ((it != address.end()) && (*it != '.'))
    {
        ++it;
    }
    if (*it != '.')
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
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
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
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

    m_address[0] = byte1;
    m_address[1] = byte2;
    m_address[2] = byte3;
    m_address[3] = byte4;
}

IPv4Address IPv4Address::Any()
{
    return IPv4Address();
}

IPv4Address IPv4Address::Localhost()
{
    IPv4Address result;

    result.m_address[0] = 127;
    result.m_address[1] = 0;
    result.m_address[2] = 0;
    result.m_address[3] = 1;

    return result;
}

string IPv4Address::toString() const
{
    string result;
    result += to_string(m_address[0]);
    result += '.';
    result += to_string(m_address[1]);
    result += '.';
    result += to_string(m_address[2]);
    result += '.';
    result += to_string(m_address[3]);
    return result;
}

}
}
