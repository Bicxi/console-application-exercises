#include "../Header/MultikBot.h"
#include <iostream>

void MultikBot::abbauen(World& world) {
    int x = this->getX();
    int y = this->getY();

    int z = world.getZ(x, y);
    int value = world.getValueZ(x, y, z);
    world.removeFeld(x, y);

    if(value < 5) {
        value *= 2;
    }

    setPoints(getPoints() + value);

    setZ(world.getZ(x, y));
    std::cout << "New Z: " << getZ() << std::endl;

    return;
}
