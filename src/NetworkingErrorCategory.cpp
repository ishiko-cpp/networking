// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkingErrorCategory.hpp"
#include "NetworkingUtilities.hpp"

using namespace Ishiko;

NetworkingErrorCategory::NetworkingErrorCategory() noexcept
    : ErrorCategory(ID(0x90b81adf90c145f2ULL, 0x9d9a4f3d35aca0faULL)) // 90b81adf-90c1-45f2-9d9a-4f3d35aca0fa
{
}

const NetworkingErrorCategory& NetworkingErrorCategory::Get() noexcept
{
    static NetworkingErrorCategory theCategory;
    return theCategory;
}

const char* NetworkingErrorCategory::name() const noexcept
{
    return "Ishiko::NetworkingErrorCategory";
}

const char* NetworkingErrorCategory::message(int ev, char* buffer, size_t len) const noexcept
{
    switch (static_cast<Value>(ev))
    {
    case Value::generic_error:
        return "generic error";

    case Value::networking_library_not_initialized:
        return "networking library not initialized";

    case Value::would_block:
        return "would block";

    default:
        return "unknown value";
    }
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

void Ishiko::Fail(NativeSocketError native_error, const std::string& message, const char* file, int line,
    Error& error) noexcept
{
    NetworkingErrorCategory::Value value = NetworkingUtilities::ConvertNativeSocketError(native_error);
    Ishiko::Fail(value, message, file, line, error);
}

bool Ishiko::operator==(const ErrorCode& error, NetworkingErrorCategory::Value value)
{
    return ((error.category().id() == NetworkingErrorCategory::Get().id()) && (error.value() == static_cast<int>(value)));
}

bool Ishiko::operator!=(const ErrorCode& error, NetworkingErrorCategory::Value value)
{
    return !(error == value);
}
