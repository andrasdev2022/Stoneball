#include "UDP.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <string>
#include <sys/stat.h>
#include "UDPConnectionFactory.h"
#include <algorithm>
#include "CommonUtil.h"
#include "android.h"

/*
 client-server communication test with nc
 server: nc -l -u 44445
 client:  nc -u ip 44445
 src: https://github.com/openbsd/src/blob/master/usr.bin/nc/netcat.c
 local_listen, remote_connect
 network traffic check in megabit per second:
 sudo tcpdump -i lo -n udp port 44445 | awk '{t=substr($1,0,8);n=substr($8,0,length($8)-1);if(t!=pt){print t,sum * 8 /1024/1024;sum=0;};sum+=n;pt=t;}'
*/

static inline int poll(struct pollfd* pfd, int nfds, int timeout) { return WSAPoll(pfd, nfds, timeout); }

namespace {
    UDPConnectionInterfaceSPtr clientConnection_;

    int64_t socketFd{};

    int timeout_connect(int s, const struct sockaddr *name, socklen_t namelen) {
        struct pollfd pfd;
        socklen_t optlen = sizeof(int);
        int optval;
        int ret;
        int timeout = -1;
        if ((ret = connect(s, name, namelen)) != 0 && errno == EINPROGRESS) {
            pfd.fd = s;
            pfd.events = POLLOUT;
            if ((ret = poll(&pfd, 1, timeout)) == 1) {
                optlen = sizeof(optval);
                if ((ret = getsockopt(s, SOL_SOCKET, SO_ERROR,
                    (char*) & optval, &optlen)) == 0) {
                    errno = optval;
                    ret = optval == 0 ? 0 : -1;
                }
            } else if (ret == 0) {
                errno = ETIMEDOUT;
                ret = -1;
            } else
                debug("ERROR: timeout_connect: poll failed");
        }
        return ret;
    }

    int64_t getSocket(const std::string& host, int port, struct addrinfo*& res, struct addrinfo*& res0) noexcept {
        if (!socketFd) {
            struct addrinfo hints;
            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;

            int64_t sockfd = -1, error, herr;
            std::string portStr = std::to_string(port);
            if ((error = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res0))) {
                debug("ERROR: UDP::getaddrinfo for host \"%s\" port %s: %s", host.c_str(), portStr.c_str(), gai_strerror(error));
                return -1;
            }

            for (res = res0; res; res = res->ai_next) {
                if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
                    continue;
                }
                u_long mode = 1;  // 1 to enable non-blocking socket
                //u_long mode = 0;  // 0 to enable blocking socket
                ioctlsocket(sockfd, FIONBIO, &mode);

                herr = getnameinfo(res->ai_addr, res->ai_addrlen, (char*)host.c_str(), NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                switch (herr) {
                case 0:
                    break;
                default:
                    debug("ERROR: UDP::sendMessage2Server getnameinfo: %ld", WSAGetLastError());
                }

                if (timeout_connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                    break;
            }

            if (!res) {
                freeaddrinfo(res0);
                res0 = nullptr;
                return -1;
            }
            socketFd = sockfd;
        }

        return socketFd;
    }
}

//std::string host("localhost");
//std::string host("192.168.1.6");
UDP::UDP() noexcept
: host_("localhost")
, port_(44444) {

}

UDP::UDP(std::string host, int port, bool resetConnection) noexcept
: host_(host)
, port_(port) {
    if(resetConnection) {
        clientConnection_ = nullptr;
        socketFd = 0;
    }
}

[[nodiscard]]bool UDP::sendMessage2Server(UDPMessageInterfaceUPtr& m) noexcept {
    if(!m->payload()) return false;
    if(!m->length()) return false;
    int64_t sockfd{};
    if(!clientConnection_) {
        struct addrinfo *res = nullptr, *res0 = nullptr;
        sockfd = getSocket(host_, port_, res, res0);
        if(sockfd == -1) {
            debug("ERROR: UDP::sendMessage2Server socket problem");
            return false;
        }
        struct sockaddr_in z;
        //debug("DBG: sizeof(z) %lu, res->ai_addrlen %lu", sizeof(z), sizeof(struct sockaddr));
        memset(&z, 0, sizeof(z));
        memcpy(&z, res->ai_addr, res->ai_addrlen);
        clientConnection_ = UDPConnectionFactory::create(sockfd, z, res->ai_addrlen);
        freeaddrinfo(res0);
    }

    clientConnection_->send(m->payload(), m->length());
    return true;
}

bool UDP::sendMessage2Client(UDPMessageInterfaceUPtr& m) noexcept {
    if(!m) return false;
    if(!m->payload()) return false;

    m->clientConnection()->send(m->payload(), m->length());
    return true;
}

const UDPConnectionInterfaceSPtr& UDP::clientConnection() noexcept {
    return clientConnection_;
}
