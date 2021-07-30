/*
    Copyright (c) 2021 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_TESTS_IPADDRESSTESTS_H_
#define _ISHIKO_NETWORKING_TESTS_IPADDRESSTESTS_H_

#include <Ishiko/Tests/Core.h>

class IPAddressTests : public Ishiko::Tests::TestSequence
{
public:
    IPAddressTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestEnvironment& environment);

private:
    static void ConstructorTest1(Ishiko::Tests::Test& test);
};

#endif
