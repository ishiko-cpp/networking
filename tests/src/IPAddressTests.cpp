/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddressTests.hpp"
#include "Ishiko/Networking/IPAddress.hpp"

using namespace Ishiko;
using namespace Ishiko::Tests;

IPAddressTests::IPAddressTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "IPAddress tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void IPAddressTests::ConstructorTest1(Test& test)
{
    IPAddress address;

    ISHIKO_PASS();
}
