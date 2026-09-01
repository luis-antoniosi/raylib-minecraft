#include "varInt.hpp"
#include <stdexcept>

namespace Network
{
    void VarInt::write(std::vector<uint8_t> &buffer, int32_t value)
    {
        uint32_t val = static_cast<uint32_t>(value);
        while ((val & ~0x7F) != 0)
        {
            buffer.push_back((static_cast<uint8_t>(val) & 0x7F) | 0x80);
            val >>= 7;
        }
        buffer.push_back(static_cast<uint8_t>(val));
    }

    int32_t VarInt::read(const uint8_t *&ptr, const uint8_t *end)
    {
        uint32_t val = 0;
        int position = 0;

        for (int i = 0; i < 5; i++)
        {
            if (ptr >= end)
                throw std::runtime_error("Buffer underflow while reading VarInt");

            uint8_t currentByte = *ptr++;
            val |= static_cast<int32_t>(currentByte & 0x7F) << position;

            if ((currentByte & 0x80) == 0)
                return val;

            position += 7;
        }

        throw std::runtime_error("VarInt is too big");
    }

    int32_t VarInt::readFromSocket(TcpClient &client)
    {
        std::vector<uint8_t> tmpBuf;
        uint8_t currByte = -1;

        do
        {
            std::vector<uint8_t> byteVec = client.receive(1);
            currByte = byteVec[0];
            tmpBuf.push_back(currByte);
        } while ((currByte & 0x80) != 0 && tmpBuf.size() < 5);

        const uint8_t *start = tmpBuf.data();
        const uint8_t *end = tmpBuf.data() + tmpBuf.size();

        return VarInt::read(start, end);
    }
}