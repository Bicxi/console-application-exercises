#include "../Header/ThreeBot.h"

void ThreeBot::abbauen(World& world) {
    int x = this->getX();
    int y = this->getY();

    for (int i = 0; i < 3; i++) {
        if(world.getZ(x,y) < 0) return;

        int z = world.getZ(x, y);
        int value = world.getValueZ(x, y, z);
        world.removeFeld(x, y);
        setPoints(getPoints() + value);
    }

    return;
}
