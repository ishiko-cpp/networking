// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTLSClientSocketTests.hpp"
#include "Ishiko/Networking/AsyncTLSClientSocket.hpp"
#include "Ishiko/Networking/NetworkConnectionsManager.hpp"
#include "Ishiko/Networking/TLSServerSocket.hpp"
#include <thread>

using namespace Ishiko;

namespace
{
    class TestCallbacks : public AsyncTLSClientSocket::Callbacks
    {
    public:
        TestCallbacks();

        void onConnectionEstablished(const Error& error, AsyncTLSClientSocket& socket) override;
        void onHandshake(const Error& error, AsyncTLSClientSocket& socket) override;
        void onReadReady(const Error& error, AsyncTLSClientSocket& socket) override;
        void onWriteReady(const Error& error, AsyncTLSClientSocket& socket) override;

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

    void TestCallbacks::onConnectionEstablished(const Error& error, AsyncTLSClientSocket& socket)
    {
        m_connection_established_called = true;
        m_connection_established_called_error = error.code();
    }

    void TestCallbacks::onHandshake(const Error& error, AsyncTLSClientSocket& socket)
    {
    }

    void TestCallbacks::onReadReady(const Error& error, AsyncTLSClientSocket& socket)
    {
    }

    void TestCallbacks::onWriteReady(const Error& error, AsyncTLSClientSocket& socket)
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

    class TestCallbacks2 : public AsyncTLSClientSocket::Callbacks
    {
    public:
        TestCallbacks2();

        void onConnectionEstablished(const Error& error, AsyncTLSClientSocket& socket) override;
        void onHandshake(const Error& error, AsyncTLSClientSocket& socket) override;
        void onReadReady(const Error& error, AsyncTLSClientSocket& socket) override;
        void onWriteReady(const Error& error, AsyncTLSClientSocket& socket) override;

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

    void TestCallbacks2::onConnectionEstablished(const Error& error, AsyncTLSClientSocket& socket)
    {
        m_connection_established_called = true;
        m_connection_established_called_error = error.code();

        socket.handshake();
    }

    void TestCallbacks2::onHandshake(const Error& error, AsyncTLSClientSocket& socket)
    {
    }

    void TestCallbacks2::onReadReady(const Error& error, AsyncTLSClientSocket& socket)
    {
    }

    void TestCallbacks2::onWriteReady(const Error& error, AsyncTLSClientSocket& socket)
    {
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

AsyncTLSClientSocketTests::AsyncTLSClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "AsyncTLSClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("connect test 1", ConnectTest1);
    append<HeapAllocationErrorsTest>("handshake test 1", HandshakeTest1);
}

void AsyncTLSClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks callbacks;
    AsyncTLSClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void AsyncTLSClientSocketTests::ConnectTest1(Test& test)
{
    boost::filesystem::path keyPath = test.context().getDataPath("test_ecdsa_key.key");
    boost::filesystem::path certificatePath = test.context().getDataPath("test_ecdsa_certificate.crt");

    std::thread serverThread(
        [&keyPath, &certificatePath]()
        {
            Error error;
            TLSServerSocket socket(IPv4Address::Localhost(), 9885, keyPath.string(), certificatePath.string(), error);
            TLSClientSocket clientSocket = socket.accept(error);

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
    AsyncTLSClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_ABORT_IF(error);

    // TODO: assign unique port number
    socket.connect(IPv4Address::Localhost(), 9885, Hostname{"example.org"});

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

void AsyncTLSClientSocketTests::HandshakeTest1(Test& test)
{
    boost::filesystem::path keyPath = test.context().getDataPath("test_ecdsa_key.key");
    boost::filesystem::path certificatePath = test.context().getDataPath("test_ecdsa_certificate.crt");

    std::thread serverThread(
        [&keyPath, &certificatePath]()
    {
        Error error;
        TLSServerSocket socket(IPv4Address::Localhost(), 9886, keyPath.string(), certificatePath.string(), error);
        TLSClientSocket clientSocket = socket.accept(error);

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
    AsyncTLSClientSocket socket{ connection_manager, callbacks, error };

    ISHIKO_TEST_ABORT_IF(error);

    // TODO: assign unique port number
    socket.connect(IPv4Address::Localhost(), 9886, Hostname{ "example.org" });

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
