/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "ErrorCategory.hpp"

namespace Ishiko
{
namespace Networking
{

const ErrorCategory& ErrorCategory::Get() noexcept
{
    static ErrorCategory theCategory;
    return theCategory;
}

const char* ErrorCategory::name() const noexcept
{
    return "Ishiko::Networking::ErrorCategory";
}

void Fail(Error& error, ErrorCategory::Value value, const std::string& message, const char* file, int line) noexcept
{
    error.fail(static_cast<int>(value), ErrorCategory::Get(), message, file, line);
}

}
}
