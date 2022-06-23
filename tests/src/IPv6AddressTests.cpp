/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPv6AddressTests.hpp"
#include "Ishiko/Networking/IPv6Address.hpp"

using namespace Ishiko;

IPv6AddressTests::IPv6AddressTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "IPv6Address tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
    append<HeapAllocationErrorsTest>("Constructor test 3", ConstructorTest3);
    append<HeapAllocationErrorsTest>("Localhost test 1", LocalhostTest1);
}

void IPv6AddressTests::ConstructorTest1(Test& test)
{
    IPv6Address address;

    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0000");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::ConstructorTest2(Test& test)
{
    Error error;
    IPv6Address address("0000:0000:0000:0000:0000:0000:0000:0000", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0000");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::ConstructorTest3(Test& test)
{
    Error error;
    IPv6Address address("a754:6bd5:897e:4565:afbc:8965:f430:a56b", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(address.value(),
        boost::multiprecision::uint128_t("222419415368176181280638945003954021739"));
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "a754:6bd5:897e:4565:afbc:8965:f430:a56b");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::LocalhostTest1(Test& test)
{
    IPv6Address address = IPv6Address::Localhost();

    ISHIKO_TEST_FAIL_IF_NEQ(address.value(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0001");
    ISHIKO_TEST_PASS();
}
