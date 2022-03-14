/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_PORT_HPP_
#define _ISHIKO_CPP_NETWORKING_PORT_HPP_

#include <string>

namespace Ishiko
{
namespace Networking
{

class Port
{
public:
    enum class WellKnown
    {
        http
    };

    Port(uint16_t number);

    unsigned short number() const;

    bool operator==(Port other) const;
    bool operator!=(Port other) const;
    std::string toString() const;

private:
    uint16_t m_number;
};

}
}

#endif
