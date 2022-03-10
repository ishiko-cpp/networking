/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPServerSocketTests.hpp"
#include "Ishiko/Networking/TCPClientSocket.hpp"
#include "Ishiko/Networking/TCPServerSocket.hpp"
#include <thread>

using namespace Ishiko;
using namespace Ishiko::Networking;
using namespace Ishiko::Tests;
using namespace std;

TCPServerSocketTests::TCPServerSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPServerSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("accept test 1", AcceptTest1);
}

void TCPServerSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), 8585, error);

    ISHIKO_FAIL_IF(error);
    ISHIKO_PASS();
}

void TCPServerSocketTests::AcceptTest1(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), 8586, error);

    ISHIKO_ABORT_IF(error);

    thread client([]()
        {
            Error error;
            TCPClientSocket clientSocket(error);
            clientSocket.connect(IPv4Address::Localhost(), 8586, error);
        }
    );

    TCPClientSocket clientSocket = socket.accept(error);

    ISHIKO_FAIL_IF(error);

    client.join();

    ISHIKO_PASS();
}
