#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

using namespace std;

#include "World.h"

class Bot
{
public:
    Bot();
    virtual ~Bot();

    virtual void abbauen(World& world) = 0;

    void setPosition(int newX, int newY, int newZ);
    void moveBot(char input, World& world);
    int getX();
    int getY();
    int getZ();

    void setPoints(int newP);
    int getPoints();
protected:

private:
    int points;
    int xPos;
    int yPos;
    int zPos;
};

#endif // BOT_H_INCLUDED
