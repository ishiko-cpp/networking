// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTLSClientSocketTests.hpp"
#include "Ishiko/Networking/AsyncTLSClientSocket.hpp"

using namespace Ishiko;

AsyncTLSClientSocketTests::AsyncTLSClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "AsyncTLSClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void AsyncTLSClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    AsyncTLSClientSocket socket{error};

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}
