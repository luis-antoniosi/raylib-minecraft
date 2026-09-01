#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <vector>
#include <string>
#include <cstdint>

namespace Network
{
    struct Packet
    {
        int32_t length;
        int32_t id;
        const uint8_t *dataStart;
        const uint8_t *dataEnd;
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
        static std::vector<uint8_t> buildHandshake(const std::string &host, uint16_t port, uint32_t nextState);
        static std::vector<uint8_t> buildLoginStart(const std::string &name);
        static std::vector<uint8_t> buildKeepAlive(const int64_t id);
        static std::vector<uint8_t> buildClientInformation(const std::string &locale, uint8_t viewDistance, uint8_t chatMode, bool chatColors, bool showCape);
        static std::vector<uint8_t> buildPlayerPositionLook(double x, double y, double z, float yaw, float pitch, uint8_t flags);

        static Packet readPacket(const std::vector<uint8_t> &data, const int32_t length);
        static PlayerPosLook readPlayerPositionLook(Packet &packet);
    };
}

#endif