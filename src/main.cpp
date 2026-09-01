#include <iostream>
#include <string>
#include "network/tcpClient.hpp"
#include "network/protocol.hpp"
#include "network/varInt.hpp"
#include "network/utils.hpp"

using namespace Network;

#define HOST "127.0.0.1"
#define PORT "25565"

// ip route show | grep default | awk '{print $3}'

enum class ConnectionState
{
    HANDSHAKE,
    STATUS,
    LOGIN,
    PLAY
};

int main()
{
    TcpClient client;
    std::string username = "adfqwfqwerf";

    if (!client.connectToServer(HOST, PORT))
        return 1;

    try
    {
        ConnectionState currentState = ConnectionState::HANDSHAKE;
        int32_t nextState = 2;
        std::vector<uint8_t> handshakePacket = Protocol::buildHandshake(HOST, std::stoi(PORT), nextState);

        client.send(handshakePacket);
        std::cout << "Handshake packet sent, size of " << handshakePacket.size() << " bytes" << std::endl;

        std::cout << "Starting login" << std::endl;
        currentState = ConnectionState::LOGIN;
        std::vector<uint8_t> loginPacket = Protocol::buildLoginStart(username);
        client.send(loginPacket);

        bool running = true;
        while (running && client.isConnected())
        {
            int32_t packetLength = VarInt::readFromSocket(client);
            std::vector<uint8_t> packetData = client.receive(packetLength);
            Packet readPacket = Protocol::readPacket(packetData, packetLength);

            if (currentState == ConnectionState::LOGIN)
            {
                if (readPacket.id == 0x00)
                {
                    std::cerr << "Disconnected by server during login" << std::endl;
                    running = false;
                }
                else if (readPacket.id == 0x01)
                {
                    std::cout << "Encryption request, ignored" << std::endl;
                }
                else if (readPacket.id == 0x02)
                {
                    std::cout << "Login was successful!" << std::endl;
                    currentState = ConnectionState::PLAY;
                }
                else if (readPacket.id == 0x03)
                {
                    std::cout << "Compression packet received and ignored" << std::endl;
                }
            }
            else if (currentState == ConnectionState::PLAY)
            {
                if (readPacket.id == 0x01)
                {
                    std::cout << "Sending client settings..." << std::endl;
                    std::vector<uint8_t> clientInfoPacket = Protocol::buildClientInformation("en-US", 10, 0, true, true);
                    client.send(clientInfoPacket);
                }
                else if (readPacket.id == 0x00)
                {
                    const uint8_t *ptr = readPacket.dataStart;
                    const uint8_t *end = readPacket.dataEnd;

                    if (ptr < end)
                    {
                        int32_t keepAliveId = VarInt::read(ptr, end);
                        std::cout << "Received keepAlive: " << keepAliveId << std::endl;

                        std::vector<uint8_t> keepAlivePacket = Protocol::buildKeepAlive(keepAliveId);
                        client.send(keepAlivePacket);
                        std::cout << "Sending keepAlive back" << std::endl;
                    }
                }
                else if (readPacket.id == 0x08)
                {
                    std::cout << "Received spawn and look" << std::endl;
                    PlayerPosLook playerInfo = Protocol::readPlayerPositionLook(readPacket);

                    std::vector<uint8_t> playerPosLookPacket = Protocol::buildPlayerPositionLook(playerInfo.x, playerInfo.y, playerInfo.z, playerInfo.yaw, playerInfo.pitch, playerInfo.flags);
                    client.send(playerPosLookPacket);
                    std::cout << "Sent spawn and look back" << std::endl;
                }
                else
                {
                    // std::cout << "Ignored packet of ID: 0x" << std::hex << (int)readPacket.id << std::dec << std::endl;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}