#include <cstring>
#include <string>

#include "packetWriter.hpp"
#include "varInt.hpp"

namespace Network
{
    PacketWriter::PacketWriter(int32_t packetId)
    {
        VarInt::write(body, packetId);
    }

    void PacketWriter::writeVarInt(int32_t value)
    {
        VarInt::write(body, value);
    }

    void PacketWriter::writeLong(int64_t value)
    {
        uint64_t tmp = static_cast<uint64_t>(value);
        tmp = htobe64(tmp);

        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        body.insert(body.end(), ptr, ptr + sizeof(uint64_t));
    }

    void PacketWriter::writeDouble(double value)
    {
        uint64_t tmp;
        std::memcpy(&tmp, &value, sizeof(double));

        tmp = htobe64(tmp);
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        body.insert(body.end(), ptr, ptr + sizeof(uint64_t));
    }

    void PacketWriter::writeFloat(float value)
    {
        uint32_t tmp;
        std::memcpy(&tmp, &value, sizeof(float));

        tmp = htonl(tmp);
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        body.insert(body.end(), ptr, ptr + sizeof(uint32_t));
    }

    void PacketWriter::writeByte(uint8_t value)
    {
        body.push_back(value);
    }

    void PacketWriter::writeInt(int32_t value)
    {
        uint32_t tmp = static_cast<uint32_t>(value);
        tmp = htobe32(tmp);

        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        body.insert(body.end(), ptr, ptr + sizeof(uint32_t));
    }

    void PacketWriter::writeUShort(uint16_t value)
    {
        body.push_back(static_cast<uint8_t>((value >> 8) && 0xFF));
        body.push_back(static_cast<uint8_t>(value & 0xFF));
    }

    void PacketWriter::writeString(const std::string &value)
    {
        writeVarInt(static_cast<int32_t>(value.size()));
        body.insert(body.end(), value.begin(), value.end());
    }

    void PacketWriter::writeBool(uint8_t value)
    {
        body.push_back(value ? 0x01 : 0x00);
    }

    std::vector<uint8_t> PacketWriter::finalize()
        const
    {
        std::vector<uint8_t> packet;
        VarInt::write(packet, static_cast<int32_t>(body.size()));
        packet.insert(packet.end(), body.begin(), body.end());

        return packet;
    }
}