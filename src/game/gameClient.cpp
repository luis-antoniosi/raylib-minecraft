#include <iostream>

#include "gameClient.hpp"
#include "protocolConstants.hpp"
#include "packetReader.hpp"

using namespace Network;

bool GameClient::start(const std::string &host, uint16_t port, const std::string &username)
{
    this->username = username;

    if (!client.connectToServer(host, std::to_string(port)))
        return false;

    client.send(Protocol::buildHandshake(host, port, ConnectionState::Login));
    client.state = ConnectionState::Login;
    client.send(Protocol::buildLoginStart(username));

    return true;
}

void GameClient::run()
{
    while (client.isConnected())
    {
        Packet packet = client.readNextPacket();
        handlePacket(packet);
    }
}

void GameClient::handlePacket(const Packet &packet)
{
    if (client.state == ConnectionState::Login)
    {
        if (packet.id == Clientbound::Login::LoginDisconnect)
        {
            PacketReader reader = packet.reader();
            std::string reason = reader.readString();
            std::cerr << "Disconnected by server during login: " << reason << std::endl;
            client.disconnect();
        }
        else if (packet.id == Clientbound::Login::EncryptionRequest)
        {
            std::cout << "Encryption request, ignored" << std::endl;
        }
        else if (packet.id == Clientbound::Login::LoginSuccess)
        {
            std::cout << "Login was successful!" << std::endl;
            client.state = ConnectionState::Play;
        }
        else if (packet.id == Clientbound::Login::EnableCompression)
        {
            std::cout << "Compression packet received and ignored" << std::endl;
        }
    }
    else if (client.state == ConnectionState::Play)
    {
        if (packet.id == Clientbound::Play::JoinGame)
        {
            // TODO: Read this packet correctly
            std::cout << "Sending client settings..." << std::endl;
            client.send(Protocol::buildClientInformation("en-US", 10, 0, true, true));
        }
        else if (packet.id == Clientbound::Play::KeepAlive)
        {
            PacketReader reader = packet.reader();
            int32_t id = reader.readVarInt();
            client.send(Protocol::buildKeepAlive(id));
        }
        else if (packet.id == Clientbound::Play::PlayerPosLook)
        {
            std::cout << "Received spawn and look" << std::endl;
            PlayerPosLook playerInfo = Protocol::readPlayerPositionLook(packet);

            // TODO: onGround is not always true..
            std::vector<uint8_t> playerPosLookPacket = Protocol::buildPlayerPositionLook(
                playerInfo.x, playerInfo.y, playerInfo.z,
                playerInfo.yaw, playerInfo.pitch,
                true);

            client.send(playerPosLookPacket);
            std::cout << "Sent spawn and look back" << std::endl;
        }
    }
    else
    {
        // std::cout << "Ignored packet of ID: 0x" << std::hex << (int)readPacket.id << std::dec << std::endl;
    }
}