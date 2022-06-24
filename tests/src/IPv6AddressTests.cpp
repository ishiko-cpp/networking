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
    append<HeapAllocationErrorsTest>("value test 1", ValueTest1);
}

void IPv6AddressTests::ConstructorTest1(Test& test)
{
    IPv6Address address;

    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0000");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::ConstructorTest2(Test& test)
{
    Error error;
    IPv6Address address("0000:0000:0000:0000:0000:0000:0000:0000", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0000");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::ConstructorTest3(Test& test)
{
    Error error;
    IPv6Address address("a754:6bd5:897e:4565:afbc:8965:f430:a56b", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "a754:6bd5:897e:4565:afbc:8965:f430:a56b");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::LocalhostTest1(Test& test)
{
    IPv6Address address = IPv6Address::Localhost();

    ISHIKO_TEST_FAIL_IF_NEQ(address.toString(), "0000:0000:0000:0000:0000:0000:0000:0001");
    ISHIKO_TEST_PASS();
}

void IPv6AddressTests::ValueTest1(Test& test)
{
    Error error;
    IPv6Address address("a754:6bd5:897e:4565:afbc:8965:f430:a56b", error);

    ISHIKO_TEST_FAIL_IF(error);

    unsigned char bytes[17];
    address.value().copyTo(bytes);
    bytes[16] = '\0';

    ISHIKO_TEST_FAIL_IF_STR_NEQ((char*)bytes, "\xa7\x54\x6b\xd5\x89\x7e\x45\x65\xaf\xbc\x89\x65\xf4\x30\xa5\x6b");
    ISHIKO_TEST_PASS();
}
