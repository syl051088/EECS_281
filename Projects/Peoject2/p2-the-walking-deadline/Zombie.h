// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#pragma once
#include "P2random.h"
#include "Mode.h"
#include "Player.h"
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

public:
    Zombie(string n, uint32_t d, uint32_t s, uint32_t h): name{n}, distance{d}, speed{s}, health{h} {
        roundsAlive = 1;
    }

    void move(const Mode &mode, string &killer, Player &player) {
        distance = distance - speed < distance? distance - speed : 0;
        if (mode.vMode) {
            cout << "Moved: " << name << " " << "(distance: " << distance << ", speed: " << speed 
                << ", health: " << health << ")\n";
        }

        if (distance == 0 && player.isAlive) {
            player.isAlive = false;
            killer = name;
        }
        ++roundsAlive;
    }
};










