#include "../Header/Bot.h"

Bot::Bot() {
    xPos = 0;
    yPos = 0;
    zPos = 9;
    points = 0;
}

Bot::~Bot() {}

void Bot::setPosition(int newX, int newY, int newZ) {
    xPos = newX;
    yPos = newY;
    zPos = newZ;
}

void Bot::moveBot(char input, World& world) {
    //check ob Input passt au�erhalb
    int x = this->getX();
    int y = this->getY();
    int z = 0;

    //bool ob comp oder selbst bei constr setzten, hier dann abfragen nach input wenn player oder random wenn comp
    switch(input) {
        case 'w':
            if(x > 0) {
                z = world.getZ(x-1, y);
                setPosition(x-1, y, z);
            }
            break;

        case 'd':
            if(x < 4) {
                z = world.getZ(x+1, y);
                setPosition(x+1, y, z);
            }
            break;

        case 'a':
            if(y > 0) {
                z = world.getZ(x, y-1);
                setPosition(x, y-1, z);
            }
            break;

        case 's':
            if(y < 4) {
                z = world.getZ(x, y+1);
                setPosition(x, y+1, z); //y++ setY
            }
        case 'e':
            break;
    }
    return;
}

int Bot::getX() { return xPos; }
int Bot::getY() { return yPos; }
int Bot::getZ() { return zPos; }

int Bot::getPoints() { return points; }

void Bot::setPoints(int newP) { points = newP; }

