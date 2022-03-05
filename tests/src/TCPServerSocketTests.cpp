/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPServerSocketTests.h"
#include "Ishiko/Networking/TCPServerSocket.h"

using namespace Ishiko;
using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

TCPServerSocketTests::TCPServerSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPServerSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void TCPServerSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), 8585, error);

    ISHIKO_FAIL_IF(error);
    ISHIKO_PASS();
}
