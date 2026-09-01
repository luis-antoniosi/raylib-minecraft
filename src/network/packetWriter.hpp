#ifndef PACKET_WRITER_HPP
#define PACKET_WRITER_HPP

#include <cstdint>
#include <vector>

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <libkern/OSByteOrder.h>
#define htobe64(x) OSSwapHostToBigInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#elif defined(_WIN32)
#include <winsock2.h>
#define htobe64(x) htonll(x)
#define be64toh(x) ntohll(x)
#define be32toh(x) ntohl(x)
#else
#include <endian.h>
#include <arpa/inet.h>
#endif

namespace Network
{
    class PacketWriter
    {
    private:
        std::vector<uint8_t> body;

    public:
        explicit PacketWriter(int32_t packetId);

        void writeVarInt(int32_t value);
        void writeLong(int64_t value);
        void writeDouble(double value);
        void writeFloat(float value);
        void writeByte(uint8_t value);
        void writeInt(int32_t value);
        void writeUShort(uint16_t value);
        void writeString(const std::string &value);
        void writeBool(uint8_t value);

        std::vector<uint8_t> finalize() const;
    };
}

#endif