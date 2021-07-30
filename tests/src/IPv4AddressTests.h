/*
    Copyright (c) 2021 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_TESTS_IPV4ADDRESSTESTS_H_
#define _ISHIKO_NETWORKING_TESTS_IPV4ADDRESSTESTS_H_

#include <Ishiko/Tests/Core.h>

class IPv4AddressTests : public Ishiko::Tests::TestSequence
{
public:
    IPv4AddressTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestEnvironment& environment);

private:
    static void ConstructorTest1(Ishiko::Tests::Test& test);
};

#endif
