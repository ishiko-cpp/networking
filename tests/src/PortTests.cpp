// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "PortTests.hpp"
#include "Ishiko/Networking/Port.hpp"

using namespace Ishiko;

PortTests::PortTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "Port tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
}

void PortTests::ConstructorTest1(Test& test)
{
    Port port(0);

    ISHIKO_TEST_FAIL_IF_NEQ(port.number(), 0);
    ISHIKO_TEST_PASS();
}

void PortTests::ConstructorTest2(Test& test)
{
    Port port("80");

    ISHIKO_TEST_FAIL_IF_NEQ(port.number(), 80);
    ISHIKO_TEST_PASS();
}
