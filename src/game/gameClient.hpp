#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <string>

#include "tcpClient.hpp"
#include "protocol.hpp"

class GameClient
{
private:
    Network::TcpClient client;
    std::string username;
    
    void handlePacket(const Network::Packet &packet);

public:
    bool start(const std::string &host, uint16_t port, const std::string &username);
    void run();
};

#endif