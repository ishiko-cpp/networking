// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocketTests.hpp"
#include "Ishiko/Networking/AsyncTCPClientSocket.hpp"
#include "Ishiko/Networking/NetworkConnectionsManager.hpp"
#include "Ishiko/Networking/TCPServerSocket.hpp"
#include <thread>

using namespace Ishiko;

namespace
{
    class TestCallbacks : public AsyncTCPClientSocket::Callbacks
    {
    public:
        TestCallbacks();

        void onConnectionEstablished(const Error& error, AsyncTCPClientSocket& socket) override;
        void onReadReady(const Error& error, AsyncTCPClientSocket& socket) override;
        void onWriteReady(const Error& error, AsyncTCPClientSocket& socket) override;

        bool connectionEstablishedCalled() const;
        ErrorCode connectionEstablishedCalledError() const;

    private:
        bool m_connection_established_called;
        ErrorCode m_connection_established_called_error;
    };

    TestCallbacks::TestCallbacks()
        : m_connection_established_called{false}
    {
    }

    void TestCallbacks::onConnectionEstablished(const Error& error, AsyncTCPClientSocket& socket)
    {
        m_connection_established_called = true;
        m_connection_established_called_error = error.code();
    }

    void TestCallbacks::onReadReady(const Error& error, AsyncTCPClientSocket& socket)
    {
    }

    void TestCallbacks::onWriteReady(const Error& error, AsyncTCPClientSocket& socket)
    {
    }

    bool TestCallbacks::connectionEstablishedCalled() const
    {
        return m_connection_established_called;
    }

    ErrorCode TestCallbacks::connectionEstablishedCalledError() const
    {
        return m_connection_established_called_error;
    }

    class TestCallbacks2 : public AsyncTCPClientSocket::Callbacks
    {
    public:
        TestCallbacks2();

        void onConnectionEstablished(const Error& error, AsyncTCPClientSocket& socket) override;
        void onReadReady(const Error& error, AsyncTCPClientSocket& socket) override;
        void onWriteReady(const Error& error, AsyncTCPClientSocket& socket) override;

        bool connectionEstablishedCalled() const;
        ErrorCode connectionEstablishedCalledError() const;

    private:
        bool m_connection_established_called;
        ErrorCode m_connection_established_called_error;
    };

    TestCallbacks2::TestCallbacks2()
        : m_connection_established_called{ false }
    {
    }

    void TestCallbacks2::onConnectionEstablished(const Error& error, AsyncTCPClientSocket& socket)
    {
        m_connection_established_called = true;
        m_connection_established_called_error = error.code();

        socket.write("a", 1);
    }

    void TestCallbacks2::onReadReady(const Error& error, AsyncTCPClientSocket& socket)
    {
    }

    void TestCallbacks2::onWriteReady(const Error& error, AsyncTCPClientSocket& socket)
    {
        socket.write("a", 1);
    }

    bool TestCallbacks2::connectionEstablishedCalled() const
    {
        return m_connection_established_called;
    }

    ErrorCode TestCallbacks2::connectionEstablishedCalledError() const
    {
        return m_connection_established_called_error;
    }
}

AsyncTCPClientSocketTests::AsyncTCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "AsyncTCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("connect test 1", ConnectTest1);
    append<HeapAllocationErrorsTest>("write test 1", WriteTest1);
}

void AsyncTCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks callbacks;
    AsyncTCPClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void AsyncTCPClientSocketTests::ConnectTest1(Test& test)
{
    std::thread serverThread(
        []()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 9685, SocketOption::none, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client closes the connection
            char buffer[1];
            clientSocket.read(buffer, 1, error);
        }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks callbacks;
    AsyncTCPClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_ABORT_IF(error);

    // TODO: assign unique port number
    socket.connect(IPv4Address::Localhost(), 9685);

    connection_manager.run(
        [](NetworkConnectionsManager& connections_manager)
        {
            return connections_manager.idle();
        });

    socket.close();

    serverThread.join();

    ISHIKO_TEST_FAIL_IF_NOT(callbacks.connectionEstablishedCalled());
    ISHIKO_TEST_FAIL_IF(callbacks.connectionEstablishedCalledError());
    ISHIKO_TEST_PASS();
}

void AsyncTCPClientSocketTests::WriteTest1(Test& test)
{
    char buffer[1];

    std::thread serverThread(
        [&buffer]()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 9686, SocketOption::none, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client writes the data
            clientSocket.read(buffer, 1, error);
        }
    );

    // TODO: this is flaky, should be able to fix once I get async server
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks2 callbacks;
    AsyncTCPClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_ABORT_IF(error);

    // TODO: assign unique port number
    socket.connect(IPv4Address::Localhost(), 9686);

    connection_manager.run(
        [](NetworkConnectionsManager& connections_manager)
        {
            return connections_manager.idle();
        });

    socket.close();
    
    serverThread.join();

    ISHIKO_TEST_FAIL_IF_NEQ(buffer[0], 'a');
    ISHIKO_TEST_PASS();
}
