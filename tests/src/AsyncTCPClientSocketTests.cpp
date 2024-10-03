// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocketTests.hpp"
#include "Ishiko/Networking/AsyncTCPClientSocket.hpp"
#include <thread>

using namespace Ishiko;

AsyncTCPClientSocketTests::AsyncTCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "AsyncTCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void AsyncTCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    AsyncTCPClientSocket socket{error};

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}
