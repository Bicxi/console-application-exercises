#include "../Header/MultikBot.h"

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

    return;
}
