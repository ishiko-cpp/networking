/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv4AddressTests.h"
#include "Ishiko/Networking/IPv4Address.h"

using namespace Ishiko;
using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

IPv4AddressTests::IPv4AddressTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "IPv4Address tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void IPv4AddressTests::ConstructorTest1(Test& test)
{
    Error error;
    IPv4Address address("127.1.2.255", error);

    ISHIKO_FAIL_IF(error);
    ISHIKO_FAIL_IF_NEQ(address.toString(), "127.1.2.255");
    ISHIKO_PASS();
}
