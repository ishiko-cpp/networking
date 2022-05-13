/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "Port.hpp"
#include <Ishiko/Text.hpp>

using namespace Ishiko;

Port::Port(uint16_t number)
    : m_number(number)
{
}

Port::Port(const std::string& number)
{
    // TODO: use exception instead of error and handle error
    Error error;
    ASCII::Convert(number, m_number, error);
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

std::string Port::toString() const
{
    return std::to_string(m_number);
}
