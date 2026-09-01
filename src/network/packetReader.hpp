#ifndef PACKET_READER_HPP
#define PACKET_READER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

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
    class PacketReader
    {
    private:
        const uint8_t *cur;
        const uint8_t *end;

    public:
        PacketReader(const uint8_t *data, size_t len);

        int32_t readVarInt();
        int64_t readLong();
        double readDouble();
        float readFloat();
        uint8_t readByte();
        int32_t readInt();
        std::string readString();
        bool readBool();

        bool hasRemaining() const;
        size_t remaining() const;
        const uint8_t *position() const;
    };
}

#endif