#include "UDPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
    #include <unistd.h>
#endif // #ifndef WIN32
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "UDPConnectionFactory.h"
#include "UDPMessageQueueInterface.h"
#include <iostream>
#include "Zlib_.h"
#include "CommonUtil.h"

/*
 client-server communication test with nc
 server: nc -l -u 44444
 client:  nc -u ip 44444
 src: https://github.com/openbsd/src/blob/master/usr.bin/nc/netcat.c
 local_listen, remote_connect
*/

UDPServer::UDPServer(UDPMessageQueueInterface* udpMessageQueue) noexcept {
    debug("DBG UDPServer::UDPServer object created");
    udpMessageQueue_ = udpMessageQueue;
}

void UDPServer::start(int port) noexcept {
    port_ = port;
    debug("DBG UDPServer::start starting UDP server");
    thread_ = std::thread(&UDPServer::run, this);
}

int local_listen(int port)
{
    struct addrinfo *res, *res0, hints;
    int sockfd = -1; //, x = 1; // ret
    int error;
    const char* host = nullptr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags |= AI_PASSIVE;

    std::string portStr = std::to_string(port);
    if((error = getaddrinfo(host, portStr.c_str(), &hints, &res0))) {
        debug("ERROR: UDPServer::getaddrinfo for host \"%s\" port %s: %s", host, portStr.c_str(), gai_strerror(error));
        return -1;;
    }

    for(res = res0; res; res = res->ai_next) {
        if((sockfd = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol)) == -1) {
            continue;
        }
/*
        ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &x, sizeof(x));
        if(ret == -1) {
            debug("ERROR: UDPServer::run setsockopt failed");
            break;
        }
*/
        if(bind(sockfd, (struct sockaddr *)res->ai_addr,res->ai_addrlen) == 0) {
            break;
        }
        close(sockfd);
        sockfd = -1;
    }
    freeaddrinfo(res0);
    return sockfd;
}

#define MAXLINE 10240
void UDPServer::run() noexcept {
    running_ = true;
    unsigned char buffer[MAXLINE];
    sockfd_ = -1;

    debug("DBG UDPServer::run server is up and running");
    sockfd_ = local_listen(port_);
    ssize_t n;
    struct sockaddr_in z;
    socklen_t len = sizeof(z);
    //struct sockaddr_storage z;
    while(!stopped_) {
        len = sizeof(z); //len is value/resuslt
        n = recvfrom(sockfd_, (char *)buffer, MAXLINE, MSG_PEEK, (struct sockaddr *) &z, &len);
        if(n > 0) {
            n = recvfrom(sockfd_, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &z, &len);
            UDPConnectionInterfaceSPtr clientConnection = UDPConnectionFactory::create(sockfd_, z, len);
            auto it = clientConnections_.find(clientConnection);
            if(it != clientConnections_.end()) {
                clientConnection->resetSocketfd(-1);
                clientConnection = *it;
            } else {
                clientConnections_.insert(clientConnection);
            }
            clientConnection->updateTimeStamp();
            udpMessageQueue_->addMessage(buffer, n, clientConnection);
        } else if(n == -1) {
            // TODO: error handling (errno values)
            if(errno != EAGAIN) {
                debug("ERROR UDPServer::run message receive error, errno: %d", errno);
            }
            usleep(1e5);
        } else {
            usleep(1e3);
        }
    }
    debug("DBG UDPServer::run server is stopping");
    running_ = false;
}

void UDPServer::join() noexcept {
    thread_.join();
}

bool UDPServer::readClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!clientConnection) return false;
    unsigned char buffer[MAXLINE];

    ssize_t n;
    socklen_t len = clientConnection->len();
    struct sockaddr_in z = clientConnection->addr();
    int64_t sockfd = clientConnection->socketfd();
    int tryesInCaseOfError = 3;
    do {
        //debug("DBG UDPServer::run readClientConnection 0");
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_PEEK, (struct sockaddr *) &z, &len);
        //debug("DBG UDPServer::run readClientConnection 1");
        if(n > 0) {
            //debug("DBG UDPServer::readClientConnection we got a msg, length: %d", n);
            n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &z, &len);

            udpMessageQueue_->addMessage(buffer, n, clientConnection);
            tryesInCaseOfError = 3;
        } else if(n == -1) {
            // TODO: error handling (errno values)
            if(errno != EAGAIN) {
                debug("ERROR UDPServer::readClientConnection message receive error, errno: %d", errno);
                if(errno == ECONNREFUSED && tryesInCaseOfError > 0) {
                    --tryesInCaseOfError;
                    debug("ERROR UDPServer::readClientConnection let's try it again");
                    continue;
                }
            } else {
                // EAGAIN, what to do now? Leave the function
                break;
            }
            debug("ERROR UDPServer::readClientConnection No more try");
            return false;
        }
    } while(n > 0);
    return true;
}

bool UDPServer::removeClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    auto it = clientConnections_.find(clientConnection);
    if(it != clientConnections_.end()) {
        // what if we don't close the server socket? ;)
        clientConnection->resetSocketfd(-1);
        clientConnections_.erase(it);
        debug("DBG client has been removed");
        return true;
    }
    return false;
}
