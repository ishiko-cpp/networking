/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv4AddressTests.hpp"
#include "Ishiko/Networking/IPv4Address.hpp"

using namespace Ishiko;
using namespace Ishiko::Tests;

IPv4AddressTests::IPv4AddressTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "IPv4Address tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
    append<HeapAllocationErrorsTest>("Localhost test 1", LocalhostTest1);
}

void IPv4AddressTests::ConstructorTest1(Test& test)
{
    IPv4Address address;

    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0.0.0.0");
    ISHIKO_TEST_PASS();
}

void IPv4AddressTests::ConstructorTest2(Test& test)
{
    Error error;
    IPv4Address address("127.1.2.255", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 0x7F0102FF);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "127.1.2.255");
    ISHIKO_TEST_PASS();
}

void IPv4AddressTests::LocalhostTest1(Test& test)
{
    IPv4Address address = IPv4Address::Localhost();

    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 0x7F000001);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "127.0.0.1");
    ISHIKO_TEST_PASS();
}
