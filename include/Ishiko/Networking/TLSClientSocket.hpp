/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKET_HPP_

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
    TLSClientSocket(Error& error) noexcept;
    TLSClientSocket(TCPClientSocket&& socket, const std::string& keyPath, const std::string& certificatePath,
        Error& error) noexcept;
    TLSClientSocket(const TLSClientSocket& other) = delete;
    TLSClientSocket(TLSClientSocket&& other) noexcept = default;
    ~TLSClientSocket() = default;

    // TODO: hostname is for SNI. Should I provide an overload for when SNI is not needed? The caller can just leave it
    // empty though
    void connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept;

    // TODO: should length be size_t, same for return value
    int read(char* buffer, int length, Error& error);
    // TODO: always blocking at the moment, if this is non-blocking then would need to return the actual number of
    // bytes sent
    // TODO: should length be size_t
    void write(const char* buffer, int length, Error& error);

private:
    friend class TLSClientSocketBotanClientImpl;
    friend class TLSClientSocketBotanServerImpl;

    class Impl
    {
    public:
        virtual void connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept = 0;
        virtual int read(char* buffer, int length, Error& error) = 0;
        virtual void write(const char* buffer, int length, Error& error) = 0;
    };

    std::unique_ptr<Impl> m_impl;
};

}

#endif
