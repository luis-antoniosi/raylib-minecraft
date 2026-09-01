#ifndef PROTOCOL_CONSTANTS_HPP
#define PROTOCOL_CONSTANTS_HPP

#include <cstdint>

namespace Network
{
    constexpr int32_t PROTOCOL_VERSION = 47;
    
    enum class ConnectionState : int32_t
    {
        Handshake,
        Status,
        Login,
        Play
    };

    namespace Serverbound::Handshake
    {
        constexpr int32_t Handshake = 0x00;
    };

    namespace Serverbound::Login
    {
        constexpr int32_t LoginStart = 0x00;
        constexpr int32_t EncryptionResponse = 0x01;
    };

    namespace Serverbound::Play
    {
        constexpr int32_t KeepAlive = 0x00;
        constexpr int32_t PlayerPosLook = 0x06;
        constexpr int32_t ClientSettings = 0x015;
    };

    namespace Clientbound::Login
    {
        constexpr int32_t LoginDisconnect = 0x00;
        constexpr int32_t EncryptionRequest = 0x01;
        constexpr int32_t LoginSuccess = 0x02;
        constexpr int32_t EnableCompression = 0x03;
    };

    namespace Clientbound::Play
    {
        constexpr int32_t KeepAlive = 0x00;
        constexpr int32_t JoinGame = 0x01;
        constexpr int32_t PlayerPosLook = 0x08;
    };
}

#endif