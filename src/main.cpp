#include <iostream>
#include <string>

#include "gameClient.hpp"

#define HOST "127.0.0.1"
#define PORT 25565

// ip route show | grep default | awk '{print $3}'

int main()
{
    GameClient game;
    if (!game.start(HOST, PORT, "yay"))
        return 1;

    game.run();

    return 0;
}