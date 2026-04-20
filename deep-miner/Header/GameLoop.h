#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED

using namespace std;

#include "World.h"
#include "Bot.h"

class GameLoop {
public:
    GameLoop();
    ~GameLoop();

    void game();
    void endGame();

    Bot* chooseBot();

    void update();
    void render();

protected:

private:
    bool running;
    World vecWorld;
    Bot* bot1;
    Bot* bot2;
    bool playerTurn;
};

#endif // GAMELOOP_H_INCLUDED

