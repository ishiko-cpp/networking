/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "PortTests.hpp"
#include "Ishiko/Networking/Port.hpp"

using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

PortTests::PortTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "Port tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void PortTests::ConstructorTest1(Test& test)
{
    Port port(0);

    ISHIKO_FAIL_IF_NEQ(port.toString(), "0");
    ISHIKO_PASS();
}
