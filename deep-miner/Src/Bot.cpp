#include "../Header/Bot.h"
#include <iostream>

using namespace std;

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

    //bool ob comp oder selbst bei constr setzten, hier dann abfragen nach input wenn player oder random wenn comp
    switch(input) {
        case 'w':
            if(xPos > 0) {
                xPos--;
            }
            break;

        case 'd':
            if(xPos < 4) {
                xPos++;
            }
            break;

        case 'a':
            if(yPos > 0) {
                yPos--;
            }
            break;

        case 's':
            if(yPos < 4) {
                yPos++;
            }
            break;

        case 'e':
            break;
    }
    return;
}

int Bot::getX() { return xPos; }
int Bot::getY() { return yPos; }
int Bot::getZ() { return zPos; }

void Bot::setX(int newX) { xPos = newX; }
void Bot::setY(int newY) { yPos = newY; }
void Bot::setZ(int newZ) { zPos = newZ; }

int Bot::getPoints() { return points; }

void Bot::setPoints(int newP) { points = newP; }

