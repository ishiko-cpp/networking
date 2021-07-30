/*
    Copyright (c) 2021 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddressTests.h"
#include "Ishiko/Networking/IPAddress.h"

using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

IPAddressTests::IPAddressTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "IPAddress tests", environment)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void IPAddressTests::ConstructorTest1(Test& test)
{
    IPAddress address;

    ISHIKO_PASS();
}
