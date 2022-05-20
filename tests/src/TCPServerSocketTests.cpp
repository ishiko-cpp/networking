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

TCPServerSocketTests::TCPServerSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPServerSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("Constructor test 2", ConstructorTest2);
    append<HeapAllocationErrorsTest>("Constructor test 3", ConstructorTest3);
    append<HeapAllocationErrorsTest>("Constructor test 4", ConstructorTest4);
    append<HeapAllocationErrorsTest>("accept test 1", AcceptTest1);
    append<HeapAllocationErrorsTest>("accept test 2", AcceptTest2);
    append<HeapAllocationErrorsTest>("close test 1", CloseTest1);
    append<HeapAllocationErrorsTest>("close test 2", CloseTest2);
}

void TCPServerSocketTests::ConstructorTest1(Test& test)
{
    TCPServerSocket socket(IPv4Address::Localhost(), 8585);

    ISHIKO_TEST_FAIL_IF_NEQ(socket.ipAddress(), IPv4Address::Localhost());
    ISHIKO_TEST_FAIL_IF_NEQ(socket.port(), 8585);
    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::ConstructorTest2(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), 8585, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(socket.ipAddress(), IPv4Address::Localhost());
    ISHIKO_TEST_FAIL_IF_NEQ(socket.port(), 8585);
    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::ConstructorTest3(Test& test)
{
    Error error;
    TCPServerSocket socket(TCPServerSocket::AllInterfaces, TCPServerSocket::AnyPort, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(socket.ipAddress(), IPv4Address(0));
    // A specific port should have been assigned by the OS
    ISHIKO_TEST_FAIL_IF_EQ(socket.port(), 0);
    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::ConstructorTest4(Test& test)
{
    Error error;
    TCPServerSocket socket(TCPServerSocket::AllInterfaces, TCPServerSocket::AnyPort, error);

    TCPServerSocket movedSocket = std::move(socket);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(movedSocket.ipAddress(), IPv4Address(0));
    // A specific port should have been assigned by the OS
    ISHIKO_TEST_FAIL_IF_EQ(movedSocket.port(), 0);
    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::AcceptTest1(Test& test)
{
    TCPServerSocket socket(IPv4Address::Localhost(), TCPServerSocket::AnyPort);

    Port listeningPort = socket.port();
    std::thread client(
        [listeningPort]()
        {
            Error error;
            TCPClientSocket clientSocket(error);
            clientSocket.connect(IPv4Address::Localhost(), listeningPort, error);
        });

    TCPClientSocket clientSocket = socket.accept();

    client.join();

    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::AcceptTest2(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    ISHIKO_TEST_ABORT_IF(error);

    Port listeningPort = socket.port();
    std::thread client(
        [listeningPort]()
        {
            Error error;
            TCPClientSocket clientSocket(error);
            clientSocket.connect(IPv4Address::Localhost(), listeningPort, error);
        });

    TCPClientSocket clientSocket = socket.accept(error);

    ISHIKO_TEST_FAIL_IF(error);

    client.join();

    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::CloseTest1(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    ISHIKO_TEST_ABORT_IF(error);

    socket.close();

    Port listeningPort = socket.port();
    TCPClientSocket clientSocket(error);

    ISHIKO_TEST_ABORT_IF(error);

    clientSocket.connect(IPv4Address::Localhost(), listeningPort, error); 

    ISHIKO_TEST_FAIL_IF_NOT(error);

    char buffer[1];
    clientSocket.read(buffer, 1, error);

    ISHIKO_TEST_FAIL_IF_NOT(error);

    ISHIKO_TEST_PASS();
}

void TCPServerSocketTests::CloseTest2(Test& test)
{
    Error error;
    TCPServerSocket socket(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    ISHIKO_TEST_ABORT_IF(error);

    Port listeningPort = socket.port();
    TCPClientSocket clientSocket(error);

    ISHIKO_TEST_ABORT_IF(error);

    clientSocket.connect(IPv4Address::Localhost(), listeningPort, error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.close();

    char buffer[1];
    clientSocket.read(buffer, 1, error);

    ISHIKO_TEST_FAIL_IF_NOT(error);

    ISHIKO_TEST_PASS();
}
