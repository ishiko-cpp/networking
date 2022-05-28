/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKETBOTANSERVERIMPL_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKETBOTANSERVERIMPL_HPP_

#include "TCPClientSocket.hpp"
#include "TLSClientSocket.hpp"
#include <botan/auto_rng.h>
#include <botan/pk_keys.h>
#include <botan/tls_server.h>
#include <string>

namespace Ishiko
{

class TLSClientSocketBotanServerImpl : public TLSClientSocket::Impl
{
public:
    TLSClientSocketBotanServerImpl(TCPClientSocket&& socket, const std::string& keyPath,
        const std::string& certificatePath, Error& error) noexcept;

    void connect(IPv4Address address, Port port, const std::string& hostname, Error& error) noexcept override;
    virtual int read(char* buffer, int length, Error& error) override;
    virtual void write(const char* buffer, int length, Error& error) override;

private:
    class BotanTLSCallbacks : public Botan::TLS::Callbacks
    {
    public:
        BotanTLSCallbacks(TCPClientSocket& socket, std::string& buffer);

        void tls_emit_data(const uint8_t data[], size_t size) override;
        void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override;
        void tls_alert(Botan::TLS::Alert alert) override;
        bool tls_session_established(const Botan::TLS::Session& session) override;

    private:
        TCPClientSocket& m_socket;
        std::string& m_buffer;
    };

    // TODO: I need to put this somewhere else and make user configurable
    class BotanCredentialsManager : public Botan::Credentials_Manager
    {
    public:
        BotanCredentialsManager(const std::string& keyPath, const std::string& certificatePath,
            Botan::AutoSeeded_RNG& rng);

        std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(const std::string& type,
            const std::string& context) override;
        std::vector<Botan::X509_Certificate> cert_chain(const std::vector<std::string>& cert_key_types,
            const std::string& type, const std::string& context) override;
        Botan::Private_Key* private_key_for(const Botan::X509_Certificate& cert, const std::string& type,
            const std::string& context) override;

    private:
        std::unique_ptr<Botan::Private_Key> m_key;
        std::string m_certificatePath;
    };

    TCPClientSocket m_socket;
    BotanTLSCallbacks m_botanTLSCallbacks;
    // TODO: what is the autoseed RNG, is it safe enough
    Botan::AutoSeeded_RNG m_rng;
    // TODO: do I need this although I said the session is never cached above?
    Botan::TLS::Session_Manager_In_Memory m_sessionManager;
    BotanCredentialsManager m_credentials;
    Botan::TLS::Strict_Policy m_policy;
    Botan::TLS::Server m_tlsServer;
    std::string m_buffer;
};

}

#endif
