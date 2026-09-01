#include "protocol.hpp"
#include "varInt.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
#include <iostream>

namespace Network
{
    std::vector<uint8_t> Protocol::buildHandshake(const std::string &host, uint16_t port, uint32_t nextState)
    {
        std::vector<uint8_t> payload;

        // packet id for a handshake
        VarInt::write(payload, 0x0);

        // now the data:

        // protocol version
        VarInt::write(payload, 47);

        // server address
        VarInt::write(payload, static_cast<int32_t>(host.length()));
        payload.insert(payload.end(), host.begin(), host.end());

        // server port; converting to big-endian and slicing (since the vector is of 8 byte integers, and a short is 16)
        port = htons(port);
        payload.push_back(static_cast<uint8_t>((port >> 8) & 0xFF));
        payload.push_back(static_cast<uint8_t>(port & 0xFF));

        // intent
        VarInt::write(payload, nextState);

        // prepend total length at the start
        std::vector<uint8_t> packet;
        VarInt::write(packet, static_cast<uint32_t>(payload.size()));
        packet.insert(packet.end(), payload.begin(), payload.end());

        return packet;
    }

    std::vector<uint8_t> Protocol::buildLoginStart(const std::string &name)
    {
        std::vector<uint8_t> payload;

        VarInt::write(payload, 0x00);

        VarInt::write(payload, static_cast<uint32_t>(name.length()));
        payload.insert(payload.end(), name.begin(), name.end());

        std::vector<uint8_t> packet;
        VarInt::write(packet, static_cast<uint32_t>(payload.size()));
        packet.insert(packet.end(), payload.begin(), payload.end());

        return packet;
    }

    std::vector<uint8_t> Protocol::buildKeepAlive(const int64_t id)
    {
        std::vector<uint8_t> payload;
        VarInt::write(payload, 0x00);

        VarInt::write(payload, static_cast<int32_t>(id));

        std::vector<uint8_t> packet;
        VarInt::write(packet, payload.size());
        packet.insert(packet.end(), payload.begin(), payload.end());
        
        return packet;
    }

    std::vector<uint8_t> Protocol::buildClientInformation(const std::string &locale, uint8_t viewDistance, uint8_t chatMode, bool chatColors, bool showCape)
    {
        std::vector<uint8_t> payload;

        // id
        VarInt::write(payload, 0x15);

        // inserting locale (string)
        VarInt::write(payload, static_cast<int32_t>(locale.size()));
        payload.insert(payload.end(), locale.begin(), locale.end());

        // inserting viewDistance and others
        payload.push_back(viewDistance);
        payload.push_back(chatMode);
        payload.push_back(chatColors ? 0x01 : 0x00);
        payload.push_back(showCape ? 0x01 : 0x00);

        int32_t length = static_cast<int32_t>(payload.size());

        std::vector<uint8_t> packet;
        VarInt::write(packet, length);
        packet.insert(packet.end(), payload.begin(), payload.end());

        return packet;
    }

    std::vector<uint8_t> Protocol::buildPlayerPositionLook(double x, double y, double z, float yaw, float pitch, uint8_t flags)
    {
        std::vector<uint8_t> payload;

        VarInt::write(payload, 0x06);
        Utils::writeDouble(payload, x);
        Utils::writeDouble(payload, y);
        Utils::writeDouble(payload, z);
        Utils::writeFloat(payload, yaw);
        Utils::writeFloat(payload, pitch);
        payload.push_back(flags);

        std::vector<uint8_t> packet;
        VarInt::write(packet, payload.size());
        packet.insert(packet.end(), payload.begin(), payload.end());

        return packet;
    }

    Packet Protocol::readPacket(const std::vector<uint8_t> &data, const int32_t length)
    {
        const uint8_t *start = data.data();
        const uint8_t *end = data.data() + data.size();

        int32_t id = VarInt::read(start, end);

        return Packet{length, id, start, end};
    }

    PlayerPosLook Protocol::readPlayerPositionLook(Packet &packet)
    {
        if (packet.id != 0x08)
            throw std::invalid_argument("Packet used is not for player position and look.");

        const uint8_t *start = packet.dataStart;
        const uint8_t *end = packet.dataEnd;

        PlayerPosLook data;
        data.x = Utils::readDouble(start, end);
        data.y = Utils::readDouble(start, end);
        data.z = Utils::readDouble(start, end);
        data.yaw = Utils::readFloat(start, end);
        data.pitch = Utils::readFloat(start, end);

        if (start + 1 > end)
            throw std::runtime_error("Buffer underflow reading PlayerPosLook flags");

        data.flags = *start++;

        return data;
    }
}