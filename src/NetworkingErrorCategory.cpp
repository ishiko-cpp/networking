/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

const NetworkingErrorCategory& NetworkingErrorCategory::Get() noexcept
{
    static NetworkingErrorCategory theCategory;
    return theCategory;
}

const char* NetworkingErrorCategory::name() const noexcept
{
    return "Ishiko::NetworkingErrorCategory";
}

void Ishiko::Throw(NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line)
{
    throw Exception(static_cast<int>(value), NetworkingErrorCategory::Get(), message, file, line);
}

void Ishiko::Fail(NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line,
    Error& error) noexcept
{
    error.fail(NetworkingErrorCategory::Get(), static_cast<int>(value), message, file, line);
}
