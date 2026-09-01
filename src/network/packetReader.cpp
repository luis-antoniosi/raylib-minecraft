#include <stdexcept>

#include "packetReader.hpp"
#include "varInt.hpp"

namespace Network
{
    PacketReader::PacketReader(const uint8_t *data, size_t len)
        : cur(data), end(data + len) {}

    int32_t PacketReader::readVarInt()
    {
        return VarInt::read(cur, end);
    }

    int64_t PacketReader::readLong()
    {
        if (cur + 8 > end)
            throw std::runtime_error("Buffer underflow while reading Long");

        uint64_t val;
        std::memcpy(&val, cur, 8);
        cur += 8;

        val = be64toh(val);

        return static_cast<uint64_t>(val);
    }

    double PacketReader::readDouble()
    {
        if (cur + 8 > end)
            throw std::runtime_error("Buffer underflow while reading Double");

        uint64_t val;
        std::memcpy(&val, cur, 8);
        cur += 8;

        val = be64toh(val);

        double result;
        std::memcpy(&result, &val, 8);
        return result;
    }

    float PacketReader::readFloat()
    {
        if (cur + 4 > end)
            throw std::runtime_error("Buffer underflow while reading Float");

        uint32_t val;
        std::memcpy(&val, cur, 4);
        cur += 4;

        val = be32toh(val);

        float result;
        std::memcpy(&result, &val, 4);
        return result;
    }

    uint8_t PacketReader::readByte()
    {
        if (cur >= end)
            throw std::runtime_error("Packet reader: read past end");

        return *cur++;
    }

    int32_t PacketReader::readInt()
    {
        if (cur + 4 > end)
            throw std::runtime_error("Buffer underflow while reading Int");

        uint32_t val;
        std::memcpy(&val, cur, 4);
        cur += 4;

        val = be32toh(val);

        return static_cast<uint32_t>(val);
    }

    std::string PacketReader::readString()
    {
        int32_t len = readVarInt();
        if (cur + len > end)
            throw std::runtime_error("Packet reader: string exceeds buffer");

        std::string str(reinterpret_cast<const char *>(cur), len);
        cur += len;

        return str;
    }

    bool PacketReader::readBool()
    {
        return (readByte() == 0x00 ? false : true);
    }

    bool PacketReader::hasRemaining()
        const
    {
        return cur < end;
    }

    size_t PacketReader::remaining()
        const
    {
        return static_cast<size_t>(end - cur);
    }

    const uint8_t *PacketReader::position()
        const
    {
        return cur;
    }
}