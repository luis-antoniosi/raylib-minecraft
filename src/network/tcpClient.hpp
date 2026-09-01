#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace Network
{
    class TcpClient
    {
    private:
        int sockfd;

    public:
        TcpClient();
        ~TcpClient();

        TcpClient(const TcpClient &) = delete;
        TcpClient &operator=(const TcpClient &) = delete;

        bool connectToServer(const std::string &host, const std::string &port);
        void disconnect();
        bool isConnected();

        void send(const std::vector<uint8_t>& data);
        std::vector<uint8_t> receive(size_t length);
    };
}

#endif