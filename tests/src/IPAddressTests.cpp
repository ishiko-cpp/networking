// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

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
    ISHIKO_TEST_FAIL_IF_NEQ(address.asIPv4Address().toString(), "127.0.0.1");
    ISHIKO_TEST_PASS();
}

void IPAddressTests::ConstructorTest3(Test& test)
{
    IPv6Address ipv6Address = IPv6Address::Localhost();
    IPAddress address(ipv6Address);

    ISHIKO_TEST_FAIL_IF(address.isIPv4());
    ISHIKO_TEST_FAIL_IF_NOT(address.isIPv6());
    ISHIKO_TEST_FAIL_IF_NEQ(address.asIPv6Address().toString(), "0000:0000:0000:0000:0000:0000:0000:0001");
    ISHIKO_TEST_PASS();
}
