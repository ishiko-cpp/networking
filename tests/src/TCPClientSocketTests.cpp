/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPClientSocketTests.hpp"
#include "Ishiko/Networking/TCPClientSocket.hpp"
#include "Ishiko/Networking/TCPServerSocket.hpp"
#include <thread>

using namespace Ishiko;
using namespace Ishiko::Tests;
using namespace std;

TCPClientSocketTests::TCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
    append<HeapAllocationErrorsTest>("getLocalAddress test 1", GetLocalIPAddressTest1);
    append<HeapAllocationErrorsTest>("getLocalPort test 1", GetLocalPortTest1);
    append<HeapAllocationErrorsTest>("getPeerAddress test 1", GetPeerIPAddressTest1);
    append<HeapAllocationErrorsTest>("getPeerPort test 1", GetPeerPortTest1);
    append<HeapAllocationErrorsTest>("write test 1", WriteTest1);
    append<HeapAllocationErrorsTest>("read test 1", ReadTest1);
    append<HeapAllocationErrorsTest>("close test 1", CloseTest1);
    append<HeapAllocationErrorsTest>("shutdown test 1", ShutdownTest1);
}

void TCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::ConstructorTest2(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    TCPClientSocket movedSocket = move(socket);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::GetLocalIPAddressTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address address = socket.getLocalIPAddress(error);

    ISHIKO_TEST_FAIL_IF_NOT(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::GetLocalPortTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    Port port = socket.getLocalPort(error);

    ISHIKO_TEST_FAIL_IF_NOT(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::GetPeerIPAddressTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address address = socket.getPeerIPAddress(error);

    ISHIKO_TEST_FAIL_IF_NOT(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::GetPeerPortTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    Port port = socket.getPeerPort(error);

    ISHIKO_TEST_FAIL_IF_NOT(error);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::WriteTest1(Test& test)
{
    char buffer[1];
    
    thread serverThread(
        [&buffer]()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 8685, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client writes the data
            clientSocket.read(buffer, 1, error);
        }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    this_thread::sleep_for(chrono::seconds(1));

    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8685, error);
    
    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address localAddress = socket.getLocalIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(localAddress, IPv4Address::Localhost());

    Port localPort = socket.getLocalPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_EQ(localPort, 0);

    IPv4Address peerAddress = socket.getPeerIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerAddress, IPv4Address::Localhost());

    Port peerPort = socket.getPeerPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerPort, 8685);

    socket.write("a", 1, error);

    ISHIKO_TEST_FAIL_IF(error);

    serverThread.join();

    ISHIKO_TEST_FAIL_IF_NEQ(buffer[0], 'a');
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::ReadTest1(Test& test)
{
    thread serverThread(
        []()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 8686, error);
            TCPClientSocket clientSocket = socket.accept(error);
            clientSocket.write("a", 1, error);
        }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    this_thread::sleep_for(chrono::seconds(1));
    
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8686, error);
    
    ISHIKO_TEST_FAIL_IF(error);

    char buffer1[1];
    int n1 = socket.read(buffer1, 1, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n1, 1);
    ISHIKO_TEST_FAIL_IF_NEQ(buffer1[0], 'a');

    serverThread.join();

    char buffer2[1];
    int n2 = socket.read(buffer2, 1, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n2, 0);
    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::CloseTest1(Test& test)
{
    // TODO: this test will hand if close() doesn't work properly. I need to make this more robust.

    thread serverThread(
        []()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 8687, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client closes the connection
            char buffer[1];
            clientSocket.read(buffer, 1, error);
        }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    this_thread::sleep_for(chrono::seconds(1));

    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8687, error);

    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address localAddress = socket.getLocalIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(localAddress, IPv4Address::Localhost());

    Port localPort = socket.getLocalPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_EQ(localPort, 0);

    IPv4Address peerAddress = socket.getPeerIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerAddress, IPv4Address::Localhost());

    Port peerPort = socket.getPeerPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerPort, 8687);

    socket.close();

    serverThread.join();

    ISHIKO_TEST_PASS();
}

void TCPClientSocketTests::ShutdownTest1(Test& test)
{
    // TODO: this test will hand if shutdown() doesn't work properly. I need to make this more robust.

    thread serverThread(
        []()
    {
        Error error;
        TCPServerSocket socket(IPv4Address::Localhost(), 8687, error);
        TCPClientSocket clientSocket = socket.accept(error);

        // This will also make sure the server stays alive until the client closes the connection
        char buffer[1];
        clientSocket.read(buffer, 1, error);
    }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    this_thread::sleep_for(chrono::seconds(1));

    Error error;
    TCPClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8687, error);

    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address localAddress = socket.getLocalIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(localAddress, IPv4Address::Localhost());

    Port localPort = socket.getLocalPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_EQ(localPort, 0);

    IPv4Address peerAddress = socket.getPeerIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerAddress, IPv4Address::Localhost());

    Port peerPort = socket.getPeerPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerPort, 8687);

    socket.shutdown(error);

    ISHIKO_TEST_FAIL_IF(error);

    serverThread.join();

    ISHIKO_TEST_PASS();
}
