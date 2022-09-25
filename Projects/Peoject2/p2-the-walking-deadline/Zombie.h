#pragma once
#include "P2random.h"
#include "ZombieGame.h"
#include "Mode.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

struct Zombie{
    string name;
    uint32_t distance;
    uint32_t speed;
    uint32_t health;
    uint32_t roundsAlive;
    bool isAlive;

public:
    Zombie(string n, uint32_t d, uint32_t s, uint32_t h): name{n}, distance{d}, speed{s}, health{h} {}

    void move(const Mode &mode) {
        distance = distance - speed < distance ? distance - speed : 0;
        if (mode.vMode) {
            cout << "Moved: " << name << " " << " (distance: " << distance << ", speed: " << speed 
                << ", health: " << health << ")\n";
        }
    }
};










