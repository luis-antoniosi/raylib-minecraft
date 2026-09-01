#ifndef VARINT_HPP
#define VARINT_HPP

#include <vector>
#include <cstdint>
#include "tcpClient.hpp"

namespace Network
{
    class VarInt
    {
    public:
        static void write(std::vector<uint8_t> &buffer, int32_t value);
        static int32_t read(const uint8_t *&ptr, const uint8_t *end);
        static int32_t readFromSocket(TcpClient& client);
    };
}

#endif