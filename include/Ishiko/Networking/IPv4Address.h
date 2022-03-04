/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_IPV4ADDRESS_H_
#define _ISHIKO_NETWORKING_IPV4ADDRESS_H_

#include <Ishiko/Errors.hpp>
#include <string>

namespace Ishiko
{
namespace Networking
{

class IPv4Address
{
public:
    IPv4Address(const std::string& address, Error& error);

    std::string toString() const;

private:
    unsigned char m_address[4];
};

}
}

#endif
