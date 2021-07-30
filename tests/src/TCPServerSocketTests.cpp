/*
    Copyright (c) 2021 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPServerSocketTests.h"
#include "Ishiko/Networking/TCPServerSocket.h"

using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

TCPServerSocketTests::TCPServerSocketTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "TCPServerSocket tests", environment)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void TCPServerSocketTests::ConstructorTest1(Test& test)
{
    TCPServerSocket socket;

    ISHIKO_PASS();
}
