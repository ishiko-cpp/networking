// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP

#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <memory>
#include <string>

namespace Ishiko
{

class TLSClientSocket
{
public:
    TLSClientSocket(int socket_options, Error& error) noexcept;
    TLSClientSocket(TCPClientSocket&& socket, const std::string& keyPath, const std::string& certificatePath,
        Error& error) noexcept;
    TLSClientSocket(const TLSClientSocket& other) = delete;
    TLSClientSocket& operator=(const TLSClientSocket& other) = delete;
    TLSClientSocket(TLSClientSocket&& other) noexcept = default;
    TLSClientSocket& operator=(TLSClientSocket&& other) = delete;
    ~TLSClientSocket() = default;

    // TODO: hostname is for SNI. Should I provide an overload for when SNI is not needed? The caller can just leave it
    // empty though
    void connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept;

    void handshake(Error& error) noexcept;

    // TODO: should length be size_t, same for return value
    int read(char* buffer, int length, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int length, Error& error);

    void close();

    IPv4Address getLocalIPAddress(Error& error) const;
    Port getLocalPort(Error& error) const;
    IPv4Address getPeerIPAddress(Error& error) const;
    Port getPeerPort(Error& error) const;

    TCPClientSocket& socket();

    // TODO: not sure about this, just a way to know if handshake is finished for now
    bool isConnected() const;

    // TODO: Not sure about this function, see how everything should fit together
    void onCallback();

private:
    friend class TLSClientSocketBotanClientImpl;
    friend class TLSClientSocketBotanServerImpl;

    class Impl
    {
    public:
        virtual ~Impl() = default;

        virtual void connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept = 0;
        virtual void handshake(Error& error) noexcept = 0;
        virtual int read(char* buffer, int length, Error& error) = 0;
        virtual void write(const char* buffer, int length, Error& error) = 0;
        virtual void close() = 0;
        virtual const TCPClientSocket& socket() const noexcept = 0;
        virtual TCPClientSocket& socket() noexcept = 0;
        virtual bool isConnected() const = 0;
        virtual void onCallback() = 0;
    };

    std::unique_ptr<Impl> m_impl;
};

}

#endif
