/*
    Copyright (c) 2022-2023 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "HostnameResolverTests.hpp"
#include "Ishiko/Networking/HostnameResolver.hpp"
#include <vector>

using namespace Ishiko;

HostnameResolverTests::HostnameResolverTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "HostnameResolver tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("resolve test 1", ResolveTest1);
    append<HeapAllocationErrorsTest>("resolve test 2", ResolveTest2);
}

void HostnameResolverTests::ConstructorTest1(Test& test)
{
    HostnameResolver resolver;

    ISHIKO_TEST_PASS();
}

void HostnameResolverTests::ResolveTest1(Test& test)
{
    HostnameResolver resolver;

    std::vector<IPv4Address> addresses;
    Error error;
    resolver.resolve("localhost", addresses, error);

    ISHIKO_TEST_ABORT_IF_NEQ(addresses.size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(addresses[0].toString(), "127.0.0.1");
    ISHIKO_TEST_FAIL_IF_NEQ(addresses[1].toString(), "127.0.0.1");
    ISHIKO_TEST_PASS();
}

void HostnameResolverTests::ResolveTest2(Test& test)
{
    HostnameResolver resolver;

    std::vector<IPv4Address> addresses;
    Error error;
    resolver.resolve("needfulsoftware.com", addresses, error);

    std::sort(addresses.begin(), addresses.end());

    ISHIKO_TEST_ABORT_IF_NEQ(addresses.size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(addresses[0].toString(), "104.21.31.121");
    ISHIKO_TEST_FAIL_IF_NEQ(addresses[1].toString(), "172.67.176.128");
    ISHIKO_TEST_PASS();
}
