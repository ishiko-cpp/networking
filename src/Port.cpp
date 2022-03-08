/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "Port.h"

using namespace std;

namespace Ishiko
{
namespace Networking
{
    
Port::Port(unsigned short number)
    : m_number(number)
{
}

unsigned short Port::number() const
{
    return m_number;
}

string Port::toString() const
{
    return to_string(m_number);
}

}
}
