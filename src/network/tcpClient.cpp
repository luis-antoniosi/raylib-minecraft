#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "tcpClient.hpp"
#include "varInt.hpp"

namespace Network
{
    TcpClient::TcpClient() : sockfd(-1) {}

    TcpClient::~TcpClient()
    {
        disconnect();
    }

    bool TcpClient::connectToServer(const std::string &host, const std::string &port)
    {
        struct addrinfo hints{}, *servinfo = nullptr, *p = nullptr;
        int rv = -1;

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0)
        {
            std::cerr << "Error: " << gai_strerror(rv);
            return false;
        }

        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("client: socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
            {
                perror("client: connect");
                ::close(sockfd);
                sockfd = -1;
                continue;
            }

            break;
        }

        freeaddrinfo(servinfo);

        if (p == nullptr)
        {
            std::cerr << "Error: Failed to connect to " << host << ":" << port << std::endl;
            return false;
        }

        std::cout << "Connected to server!" << std::endl;
        return true;
    }

    void TcpClient::disconnect()
    {
        if (sockfd != -1)
        {
            ::close(sockfd);
            sockfd = -1;
            std::cout << "Closed socket." << std::endl;
        }
    }

    bool TcpClient::isConnected()
    {
        return sockfd >= 0;
    }

    void TcpClient::send(const std::vector<uint8_t>& data)
    {
        if (!isConnected())
            throw std::runtime_error("Cannot send data: not connected");

        size_t totalSent = 0;

        while (totalSent < data.size())
        {
            ssize_t bytesSent = ::send(sockfd, data.data() + totalSent, data.size() - totalSent, 0);
            if (bytesSent < 0)
            {
                perror("send");
                throw std::runtime_error("Failed to send data over socket.");
            }

            totalSent += bytesSent;
        }
    }

    std::vector<uint8_t> TcpClient::receive(size_t length)
    {
        if (!isConnected())
            throw std::runtime_error("Cannot receive data: not connected");

        std::vector<uint8_t> buffer(length);
        ssize_t totalReceived = 0;
       
        while (totalReceived < length)
        {
            ssize_t bytesReceived = ::recv(sockfd, buffer.data() + totalReceived, buffer.size() - totalReceived, 0);
            if (bytesReceived < 0)
            {
                perror("recv");
                throw std::runtime_error("Failed to receive data over socket.");
            }

            if (bytesReceived == 0)
                throw std::runtime_error("Connection closed by remote host.");

            totalReceived += bytesReceived;
        }

        return buffer;
    }

    Packet TcpClient::readNextPacket()
    {
        int32_t length = VarInt::readFromSocket(*this);
        std::vector<uint8_t> buffer = receive(length);
        return Protocol::readPacket(buffer, length);
    }
}