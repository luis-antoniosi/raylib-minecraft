#include <iostream>

#include "protocol.hpp"
#include "protocolConstants.hpp"
#include "packetReader.hpp"
#include "packetWriter.hpp"

namespace Network
{
    std::vector<uint8_t> Protocol::buildHandshake(const std::string &host, uint16_t port, ConnectionState nextState)
    {
        PacketWriter writer(Serverbound::Handshake::Handshake);
        std::vector<uint8_t> payload;

        writer.writeVarInt(PROTOCOL_VERSION);
        writer.writeString(host);
        writer.writeUShort(port);
        writer.writeVarInt(static_cast<int32_t>(nextState));

        return writer.finalize();
    }

    std::vector<uint8_t> Protocol::buildLoginStart(const std::string &name)
    {
        PacketWriter writer(Serverbound::Login::LoginStart);
        writer.writeString(name);

        return writer.finalize();
    }

    std::vector<uint8_t> Protocol::buildKeepAlive(const int32_t id)
    {
        PacketWriter writer(Serverbound::Play::KeepAlive);
        writer.writeVarInt(id);
        
        return writer.finalize();
    }

    std::vector<uint8_t> Protocol::buildClientInformation(const std::string &locale, uint8_t viewDistance, uint8_t chatMode, bool chatColors, bool showCape)
    {
        PacketWriter writer(Serverbound::Play::ClientSettings);

        writer.writeString(locale);
        writer.writeByte(viewDistance);
        writer.writeByte(chatMode);
        writer.writeBool(chatColors);
        writer.writeBool(showCape);

        return writer.finalize();
    }

    std::vector<uint8_t> Protocol::buildPlayerPositionLook(double x, double y, double z, float yaw, float pitch, bool onGround)
    {
        PacketWriter writer(Serverbound::Play::PlayerPosLook);

        writer.writeDouble(x);
        writer.writeDouble(y);
        writer.writeDouble(z);
        writer.writeFloat(yaw);
        writer.writeFloat(pitch);
        writer.writeBool(onGround);

        return writer.finalize();
    }

    Packet Protocol::readPacket(const std::vector<uint8_t> &buffer, const int32_t length)
    {
        PacketReader reader(buffer.data(), buffer.size());

        Packet packet;
        packet.id = reader.readVarInt();
        packet.data.assign(reader.position(), buffer.data() + buffer.size());
        packet.length = length;

        return packet;
    }

    PlayerPosLook Protocol::readPlayerPositionLook(const Packet &packet)
    {
        if (packet.id != Clientbound::Play::PlayerPosLook)
            throw std::invalid_argument("Packet used is not for player position and look.");

        PacketReader reader = packet.reader();

        PlayerPosLook data;
        data.x = reader.readDouble();
        data.y = reader.readDouble();
        data.z = reader.readDouble();
        data.yaw = reader.readFloat();
        data.pitch = reader.readFloat();
        data.flags = reader.readByte();

        return data;
    }
}