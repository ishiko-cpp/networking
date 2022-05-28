/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKETBOTANCLIENTIMPL_HPP_
#define _ISHIKO_CPP_NETWORKING_TLSCLIENTSOCKETBOTANCLIENTIMPL_HPP_

#include "TCPClientSocket.hpp"
#include "TLSClientSocket.hpp"
#undef min  // TODO: track down where I include <windows.h> and sort this out
#include <botan/auto_rng.h>
#include <botan/certstor_system.h>
#include <botan/tls_callbacks.h>
#include <botan/tls_client.h>
#include <botan/tls_policy.h>
#include <botan/tls_session_manager.h>
#include <Ishiko/Errors.hpp>
#include <string>
#include <vector>

namespace Ishiko
{

class TLSClientSocketBotanClientImpl : public TLSClientSocket::Impl
{
public:
    TLSClientSocketBotanClientImpl(Error& error) noexcept;

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
        void tls_verify_cert_chain(const std::vector<Botan::X509_Certificate>& cert_chain,
            const std::vector<std::shared_ptr<const Botan::OCSP::Response>>& ocsp,
            const std::vector<Botan::Certificate_Store*>& trusted_roots, Botan::Usage_Type usage,
            const std::string& hostname, const Botan::TLS::Policy& policy) override;

    private:
        TCPClientSocket& m_socket;
        std::string& m_buffer;
    };

    // TODO: I need to put this somewhere else and make user configurable
    class BotanCredentialsManager : public Botan::Credentials_Manager
    {
    public:
        BotanCredentialsManager();

        std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(const std::string& type,
            const std::string& context) override;
        std::vector<Botan::X509_Certificate> cert_chain(const std::vector<std::string>& cert_key_types,
            const std::string& type, const std::string& context) override;
        Botan::Private_Key* private_key_for(const Botan::X509_Certificate& cert, const std::string& type,
            const std::string& context) override;

    private:
        std::vector<Botan::Certificate_Store*> m_stores;
    };

    TCPClientSocket m_socket;
    BotanTLSCallbacks m_botanTLSCallbacks;
    // TODO: what is the autoseed RNG, is it safe enough
    Botan::AutoSeeded_RNG m_rng;
    // TODO: do I need this although I said the session is never cached above?
    Botan::TLS::Session_Manager_In_Memory m_sessionManager;
    BotanCredentialsManager m_credentials;
    Botan::TLS::Strict_Policy m_policy;
    // TODO: I needed to make this a pointer because the port is only known when connect(...) is called.
    std::unique_ptr<Botan::TLS::Client> m_tlsClient;
    std::string m_buffer;
};

}

#endif
