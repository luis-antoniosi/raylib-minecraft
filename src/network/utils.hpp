#include <cstdint>
#include <vector>

namespace Network
{
    class Utils
    {
    public:
        static int64_t readLong(const uint8_t *&start, const uint8_t *end);
        static double readDouble(const uint8_t *&start, const uint8_t *end);
        static float readFloat(const uint8_t *&start, const uint8_t *end);

        static void writeDouble(std::vector<uint8_t> &buffer, double value);
        static void writeFloat(std::vector<uint8_t> &buffer, float value);
    };
}