#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <vector>
#include <string>
#include <cstdint>

#include "protocolConstants.hpp"
#include "packetReader.hpp"

namespace Network
{
    struct Packet
    {
        int32_t length; // packet id + data
        int32_t id;
        std::vector<uint8_t> data;

        PacketReader reader() const
        {
            return PacketReader(data.data(), data.size());
        }
    };

    struct PlayerPosLook
    {
        double x, y, z;
        float yaw, pitch;
        uint8_t flags;
    };

    class Protocol
    {
    public:
        static std::vector<uint8_t> buildHandshake(const std::string &host, uint16_t port, ConnectionState nextState);
        static std::vector<uint8_t> buildLoginStart(const std::string &name);
        static std::vector<uint8_t> buildKeepAlive(const int32_t id);
        static std::vector<uint8_t> buildClientInformation(const std::string &locale, uint8_t viewDistance, uint8_t chatMode, bool chatColors, bool showCape);
        static std::vector<uint8_t> buildPlayerPositionLook(double x, double y, double z, float yaw, float pitch, bool onGround);

        static Packet readPacket(const std::vector<uint8_t> &buffer, const int32_t length);
        static PlayerPosLook readPlayerPositionLook(const Packet &packet);
    };
}

#endif