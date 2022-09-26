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
    priority_queue<Zombie*, vector<Zombie*>, ETA> zombiePQ;
    priority_queue<uint32_t, vector<uint32_t>> first;
    priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> second;
    
    deque<Zombie> zombieDeq;
    deque<Zombie*> deadDeq;

    Mode mode;

    uint32_t quiverCapacity;
    Player player;

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

    void test01() {
        readHeader();
        
        do {
            ++currentRound;
            // 1. Print round
            if (mode.vMode) {
                cout << "Round: " << currentRound << "\n";
            }

            // 2. Refill quiver
            player.refill(quiverCapacity);

            // 3.1 Update zombie position
            for (Zombie &zombie : zombieDeq) {
                if (zombie.health != 0) {
                zombie.move(mode, killer, player);
                }
            }

            // 3.2 Player dies
            if (!player.isAlive) {
                cout << "DEFEAT IN ROUND " << currentRound << "! " << killer << " ate your brains!" << "\n";
                break;
            }

            // 5. New zombies appear
            if (currentRound > zombieRound) {
                readRound();
            }
            if (currentRound == zombieRound) {
                readNewZombie();
            }
            // 6. Player shoots zombies
            playerAttack();
            
            // 7. median print
            if (mode.mMode && !deadDeq.empty()) {
                cout << "At the end of round " << currentRound << ", the median zombie lifetime is " << getMedian() << "\n";
            }

            // win
            if (!futureRound && zombiePQ.empty()) {
                cout << "VICTORY IN ROUND " << currentRound << "! " << lastEnemy << " was the last zombie." << "\n";
                break;
            }
        } while (player.isAlive || !zombiePQ.empty() || zombieRound > currentRound || futureRound);

        // s mode
        if (mode.sMode) {
            printStats();
        }
    }
};