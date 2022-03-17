/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "NetworkingErrorCategory.hpp"

namespace Ishiko
{

const NetworkingErrorCategory& NetworkingErrorCategory::Get() noexcept
{
    static NetworkingErrorCategory theCategory;
    return theCategory;
}

const char* NetworkingErrorCategory::name() const noexcept
{
    return "Ishiko::NetworkingErrorCategory";
}

void Fail(Error& error, NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line) noexcept
{
    error.fail(static_cast<int>(value), NetworkingErrorCategory::Get(), message, file, line);
}

}
