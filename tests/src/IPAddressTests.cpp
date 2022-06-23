/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddressTests.hpp"
#include "Ishiko/Networking/IPAddress.hpp"
#include "Ishiko/Networking/IPv4Address.hpp"
#include "Ishiko/Networking/IPv6Address.hpp"

using namespace Ishiko;

IPAddressTests::IPAddressTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "IPAddress tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
    append<HeapAllocationErrorsTest>("Constructor test 3", ConstructorTest3);
}

void IPAddressTests::ConstructorTest1(Test& test)
{
    IPAddress address;

    ISHIKO_TEST_FAIL_IF_NOT(address.isIPv4());
    ISHIKO_TEST_FAIL_IF(address.isIPv6());
    ISHIKO_TEST_PASS();
}

void IPAddressTests::ConstructorTest2(Test& test)
{
    IPv4Address ipv4Address = IPv4Address::Localhost();
    IPAddress address(ipv4Address);

    ISHIKO_TEST_FAIL_IF_NOT(address.isIPv4());
    ISHIKO_TEST_FAIL_IF(address.isIPv6());
    ISHIKO_TEST_PASS();
}

void IPAddressTests::ConstructorTest3(Test& test)
{
    IPv6Address ipv6Address = IPv6Address::Localhost();
    IPAddress address(ipv6Address);

    ISHIKO_TEST_FAIL_IF(address.isIPv4());
    ISHIKO_TEST_FAIL_IF_NOT(address.isIPv6());
    ISHIKO_TEST_PASS();
}
