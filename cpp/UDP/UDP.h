#ifndef __UDP_H__
#define __UDP_H__

#include <stddef.h>
#include <memory>
#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
#endif
#include "UDPMessageInterface.h"
#include <string>

class UDP {
public:
    UDP() noexcept;
    UDP(std::string host, int port, bool resetConnection) noexcept;

    class Message : public UDPMessageInterface {
    public:
        virtual ~Message() noexcept = default;
        size_t length() const noexcept override {return length_;}
        const unsigned char* payload() noexcept override {return payload_.get();}
        // one package, one message
        // first byte is the message id
        const UDPConnectionInterfaceSPtr& clientConnection() const override {return clientConnection_;}
        void setClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) override {clientConnection_ = clientConnection;}

    protected:
        std::unique_ptr<unsigned char[]> payload_;
        size_t length_{0};
        UDPConnectionInterfaceSPtr clientConnection_;
    };

    [[nodiscard]]bool sendMessage2Server(UDPMessageInterfaceUPtr& m) noexcept;
    bool sendMessage2Client(UDPMessageInterfaceUPtr& m) noexcept;
    static const UDPConnectionInterfaceSPtr& clientConnection() noexcept;

private:
    std::string host_;
    int port_;
};

constexpr const char* loadBalancerHost = "192.168.1.7";
constexpr int loadBalancerPort = 44444;
#endif // __UDP_H__

