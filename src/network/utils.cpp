#include "utils.hpp"
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>

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
#endif

namespace Network
{
    int64_t Utils::readLong(const uint8_t *&start, const uint8_t *end)
    {
        if (start + 8 > end)
            throw std::runtime_error("Buffer underflow while reading Long");

        uint64_t val;
        std::memcpy(&val, start, 8);
        start += 8;

        val = be64toh(val);

        return static_cast<uint64_t>(val);
    }

    double Utils::readDouble(const uint8_t *&start, const uint8_t *end)
    {
        if (start + 8 > end)
            throw std::runtime_error("Buffer underflow while reading Double");

        uint64_t val;
        std::memcpy(&val, start, 8);
        start += 8;

        val = be64toh(val);

        double result;
        std::memcpy(&result, &val, 8);
        return result;
    }

    float Utils::readFloat(const uint8_t *&start, const uint8_t *end)
    {
        if (start + 4 > end)
            throw std::runtime_error("Buffer underflow while reading Float");

        uint32_t val;
        std::memcpy(&val, start, 4);
        start += 4;

        val = be32toh(val);

        float result;
        std::memcpy(&result, &val, 4);
        return result;
    }

    void Utils::writeDouble(std::vector<uint8_t> &buffer, double value)
    {
        uint64_t tmp;
        std::memcpy(&tmp, &value, sizeof(double));

        tmp = htobe64(tmp);
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        buffer.insert(buffer.end(), ptr, ptr + 8);
    }

    void Utils::writeFloat(std::vector<uint8_t> &buffer, float value)
    {
        uint32_t tmp;
        std::memcpy(&tmp, &value, sizeof(float));

        tmp = htonl(tmp);
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&tmp);
        buffer.insert(buffer.end(), ptr, ptr + 4);
    }
}
