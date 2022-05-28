/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "HostnameResolverTests.hpp"

using namespace Ishiko;

HostnameResolverTests::HostnameResolverTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "HostnameResolver tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void HostnameResolverTests::ConstructorTest1(Test& test)
{
}
