// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#pragma once
#include "Zombie.h"

struct MostActive {
    bool operator() (const Zombie &z1, const Zombie &z2) {
        if (z1.roundsAlive > z2.roundsAlive) {
            return true;
        } else if (z1.roundsAlive < z2.roundsAlive) {
            return false;
        } else {
            return z1.name < z2.name;
        }
    }
};

struct SwapName {
    bool operator() (const Zombie &z1, const Zombie &z2) {
        if (z1.roundsAlive > z2.roundsAlive) {
            return true;
        } else if (z1.roundsAlive < z2.roundsAlive) {
            return false;
        } else {
            return z1.name > z2.name;
        }
    }
};

struct ETA {
    bool operator() (const Zombie *z1, const Zombie *z2) {
        if (z1->distance / z1->speed > z2->distance / z2->speed) {
            return true;
        } else if (z1->distance / z1->speed < z2->distance / z2->speed) {
            return false;
        } else if (z1->health > z2->health) {
            return true;
        } else if (z1->health < z2->health) {
            return false;
        }
        return z1->name > z2->name;
    }
};