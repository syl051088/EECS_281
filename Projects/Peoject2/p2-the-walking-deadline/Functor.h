#pragma once
#include "Zombie.h"

struct MostActive {
    bool operator() () {
        return false;
    }
};


struct LeastActive {
    bool operator() () {
        return false;
    }
};


struct FirstKill {
    bool operator() () {
        return false;
    }
};


struct LastKill {
    bool operator() () {
        return false;
    }
};

struct ETA {
    bool operator() (Zombie *z1, Zombie *z2) {
        if (z1->distance / z1->speed < z2->distance / z2->speed) {
            return true;
        } else if (z1->distance / z1->speed > z2->distance / z2->speed) {
            return false;
        } 
        return z1->name < z2->name;
    }
};