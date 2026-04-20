#include "../Header/SortBot.h"

void SortBot::abbauen(World& world) {
    int x = this->getX();
    int y = this->getY();

    world.sortSpalte(x, y);

    int z = world.getZ(x, y);
    int value = world.getValueZ(x, y, z);
    world.removeFeld(x, y);

    setPoints(getPoints() + value);
}
