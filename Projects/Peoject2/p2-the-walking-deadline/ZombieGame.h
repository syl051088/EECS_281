#pragma once
#include <iostream>
#include "Player.h"
#include <queue>
#include <vector>
#include "Functor.h"
#include "Mode.h"
#include "P2random.h"
using namespace std;



class Game {
public:
    priority_queue<uint32_t, vector<uint32_t>> first;
    priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> second;
    priority_queue<Zombie*, vector<Zombie*>, ETA> zombiePQ;
    deque<Zombie*> zombieDeq;

    Mode mode;

    uint32_t quiverCapacity;
    Player player;

    uint32_t randomSeed;
    uint32_t maxRandDistance;
    uint32_t maxRandSpeed;
    uint32_t maxRandHealth;
    uint32_t currentRound;
    uint32_t zombieRound = 0;
    

public:
    Game() {

    }
    void getMode(int argc, char * argv[]);

    void readHeader();

    void readRound();

    void createZombie(const string &name, uint32_t distance, uint32_t speed, uint32_t health);

    void createPlayer();

    void getOutput();

    void readNewZombie();



    void printHelp(char *argv[]);

    uint32_t getMedian();

    void medianTracker();

    void test01() {
        readHeader();
        
        while (currentRound <= 7) {
            if (mode.vMode) {
                cout << "Round: " << currentRound << "\n";
            }

            if (currentRound > zombieRound) {
                readRound();
            }

            
            for (Zombie* zombie : zombieDeq) {
                zombie->move(mode);
            }
            if (currentRound == zombieRound) {
                readNewZombie();
            }
            currentRound++;
        }
    }

    

};