#include "../Header/GameLoop.h"
#include "../Header/Bot.h"
#include "../Header/World.h"
#include "../Header/SortBot.h"
#include "../Header/ThreeBot.h"
#include "../Header/MultikBot.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

GameLoop::GameLoop() {}
GameLoop::~GameLoop() {}

void GameLoop::game() {
    running = true;

    cout << "Welcome to Deep Miner!\n" << endl;
    cout << "Do you want to:\n1: play yourself or\n2: watch the Computer?\n" << endl;

    while(true) {
        string input;
        getline(cin, input);

        if (input.length() != 1 || !isdigit(input[0])) { //Validierung
            cout << "Invalid input, please try again." << endl;
            continue;
        }

        int choice = input[0] - '0'; // char 1 zu int 1

        if(choice < 1 || choice > 2) {
            cout << "Invalid number, please enter 1 or 2." << endl;
            continue;
        }

        switch(choice) {
        case 1:
            {
            bot1 = chooseBot();

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, 2);
            int compChoice = dist(gen);

            switch(compChoice)
            {
                case 0: bot2 = new SortBot(); cout << "Computer chose SortBot"; break;
                case 1: bot2 = new ThreeBot(); cout << "Computer chose ThreeBot"; break;
                case 2: bot2 = new MultikBot(); cout << "Computer chose MultikBot"; break;
            }

            playerTurn = true;

            break;
            }
        case 2:
            {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, 2);
            int compChoice = dist(gen);

            int compChoice1 = dist(gen);
            int compChoice2 = dist(gen);

            switch(compChoice1)
            {
                case 0: bot1 = new SortBot(); cout << "Comp1 chose SortBot"; break;
                case 1: bot1 = new ThreeBot(); cout << "Comp1 chose ThreeBot"; break;
                case 2: bot1 = new MultikBot(); cout << "Comp1 chose MultikBot"; break;
            }

            switch(compChoice2)
            {
                case 0: bot2 = new SortBot(); cout << "Comp2 chose SortBot"; break;
                case 1: bot2 = new ThreeBot(); cout << "Comp2 chose ThreeBot"; break;
                case 2: bot2 = new MultikBot(); cout << "Comp1 chose MultikBot"; break;
            }

            break;
            }
        default:
            cout << "Invalid input, please try again." << endl;
            continue;
        }

        break;
    }

    //GAMELOOP
    while(running) {
        render();
        update();
    }

}

void GameLoop::render(){
    cout << "\n------------------------\n";

    vecWorld.printWorld();

    cout << "\nBOT1: ("
         << bot1->getX() << ", "
         << bot1->getY() << ", "
         << bot1->getZ() << ") Score: "
         << bot1->getPoints();

    cout << "\nBOT2: ("
         << bot2->getX() << ", "
         << bot2->getY() << ", "
         << bot2->getZ() << ") Score: "
         << bot2->getPoints();
}

void GameLoop::update()
{
    char input;

    if(playerTurn)
    {
        cout << "\nYOUR TURN (w/a/s/d/e for stying) or x for exit: ";
        cin >> input;

        if(input == 'x')
        {
            endGame();
            return;
        }

        bot1->moveBot(input, vecWorld);
        bot1->abbauen(vecWorld);
    }
    else
    {
        cout << "\nCOMPUTER TURN...\n";

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 4);

        char moves[5] = {'w','a','s','d','e'};
        char compMove = moves[dist(gen)];

        bot2->moveBot(compMove, vecWorld);
        bot2->abbauen(vecWorld);

        cout << "Computer moved: " << compMove << endl;
    }

    playerTurn = !playerTurn;
}

Bot* GameLoop::chooseBot() {
    cout << "Which Bot do you want?" << endl;
    cout << "1: SortBot - sorts the Mines before destroying\n";
    cout << "2: ThreeBot - destroys 3 Mines\n";
    cout << "3: MultikBot - if value is < 4, multi with 2\n";

    int choice = 0;

    while(true) {
        string input;
        getline(cin, input);

        if (input.length() != 1 || !isdigit(input[0])) { //Validierung
            cout << "Invalid input, please try again" << endl;
            continue;
        }

        choice = input[0] - '0'; //char zu int

        if (choice > 3 || choice < 1) {
            cout << "Invalid input, please enter 1, 2 or 3." << endl;
            continue;
        }

        break;
    }

    switch(choice) {
    case 1:
        return new SortBot();
    case 2:
        return new ThreeBot();
    case 3:
        return new MultikBot();
    }
}

void GameLoop::endGame() {
    running = false;
}
