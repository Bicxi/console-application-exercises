#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

using namespace std;

#include <vector>

class World{
public:
    World();
    ~World();

    void printWorld();

    int getZ(int x ,int y);
    int getValueZ(int x, int y, int z);
    void removeFeld(int x, int y);
    void setValue(int x, int y, int z, int val);

    void sortSpalte(int x, int y);

protected:

private:
    int length;
    int width;
    int height;
    vector<vector<vector<int>>> vecWorld;
};


#endif // WORLD_H_INCLUDED
