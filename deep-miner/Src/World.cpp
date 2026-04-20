#include "../Header/World.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

World::World() {
    length = 5;
    width = 5;
    height = 10;

    vecWorld = vector<vector<vector<int>>>(length, vector<vector<int>>(width, vector<int>(height)));

    // Zufallsgenerator 1 - 9
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 9);

    // Bef�llen
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < length; x++)
        {
            for (int z = 0; z < height; z++)
            {
                vecWorld[x][y][z] = dist(gen);
            }
        }
    }
}

World::~World() {}

void World::printWorld() {
    /*Ausgabe alles
    cout << "Ausgabe ganze World" << endl;
    for (int x = 0; x < length; x++)
    {
        cout << "Laenge:\n";
        for (int y = 0; y < width; y++)
        {
            cout << "Breite: ";
            for (int z = 0; z < height; z++)
            {
                cout << vecWorld[x][y][z] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }*/

    cout << "\n\nOberste Blöcke" << endl;
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < length; x++)
        {
            //cout << x << " "<< y << ": "; x-Achse zur seite, y-Achse nach unten
            cout << vecWorld[x][y].back() << " ";
        }
        cout << endl;
    }
}

int World::getZ(int x, int y) {
    //oberstes element (letztes):
    if(!vecWorld[x][y].empty()) {
        int z = vecWorld[x][y].size() - 1;
        return z;
    }
    return -1; // Spalte leer
}

int World::getValueZ(int x, int y, int z) {
    int value = vecWorld[x][y][z]; //mit .back()
    return value;
}

void World::removeFeld(int x, int y) {
    if(!vecWorld[x][y].empty()) {
        vecWorld[x][y].pop_back();
    }
}

void World::setValue(int x, int y, int z, int val) {
    vecWorld.at(x).at(y).at(z) = val;
}

void World::sortSpalte(int x, int y) {
    if(!vecWorld[x][y].empty())
        std::sort(vecWorld[x][y].begin(), vecWorld[x][y].end());
    else
        cout << "Spalte ist leer" << endl;
}

