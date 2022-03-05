/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_PORT_H_
#define _ISHIKO_NETWORKING_PORT_H_

#include <string>

namespace Ishiko
{
namespace Networking
{

class Port
{
public:
    Port(unsigned short number);

    unsigned short number() const;

    std::string toString() const;

private:
    unsigned short m_number;
};

}
}

#endif
