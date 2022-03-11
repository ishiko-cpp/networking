/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "Port.hpp"

using namespace std;

namespace Ishiko
{
namespace Networking
{

Port::Port(uint16_t number)
    : m_number(number)
{
}

Port Port::Any()
{
    return Port(0);
}

uint16_t Port::number() const
{
    return m_number;
}

bool Port::operator==(Port other) const
{
    return (m_number == other.m_number);
}

bool Port::operator!=(Port other) const
{
    return (m_number != other.m_number);
}

string Port::toString() const
{
    return to_string(m_number);
}

}
}
