// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#pragma once
#include <iostream>
#include "Player.h"
#include <queue>
#include <deque>
#include <vector>
#include <string>
#include "Functor.h"
#include "Mode.h"
#include "Zombie.h"
#include "P2random.h"
using namespace std;



class Game {
public:
    deque<Zombie> zombieDeq;
    priority_queue<Zombie*, vector<Zombie*>, ETA> zombiePQ;
    deque<Zombie*> deadDeq;
    priority_queue<uint32_t, vector<uint32_t>> first;
    priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> second;

    Mode mode;

    Player player;
    uint32_t quiverCapacity;

    uint32_t randomSeed;
    uint32_t maxRandDistance;
    uint32_t maxRandSpeed;
    uint32_t maxRandHealth;
    uint32_t currentRound;
    uint32_t zombieRound;

    string killer;
    string lastEnemy;
    bool futureRound;
    

public:
    Game() {
        zombieRound = 0;
        currentRound = 0;
        futureRound = true;
    }
    void getMode(int argc, char * argv[]);

    void readHeader();

    void readRound();

    void createZombie(const string &name, uint32_t distance, uint32_t speed, uint32_t health);

    void getOutput();

    void readNewZombie();

    void printHelp(char *argv[]);

    void playerAttack();

    uint32_t getMedian();

    void updateMedian(uint32_t roundsAlive);

    void printStats();

    void test01();
};