// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include <iostream>
#include <getopt.h>
#include "ZombieGame.h"
#include "Zombie.h"
#include "xcode_redirect.hpp"
#include "Player.h"
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include "Functor.h"
#include "P2random.h"
using namespace std;

void Game::getMode(int argc, char * argv[]) {

    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "verbose",  no_argument,       nullptr, 'v' },
        { "statistics", required_argument,       nullptr, 's' },
        { "median",   no_argument,       nullptr, 'm' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr,  0,                 nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "vs:mh", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'v':
                mode.vMode = true;
                break;
            case 's':
                mode.sMode = true;
                mode.sModeN = stoi(optarg);
                break;
            case 'm':
                mode.mMode = true;
                break;
            
            default:
                cerr << "Unknown command line option" << endl;
                exit(1);
        } // switch
    } // while
} // getMode

void Game::readHeader() {
    string junk;
    getline(cin, junk);
    cin >> junk >> quiverCapacity >> junk >> randomSeed >> junk >> maxRandDistance 
        >> junk >> maxRandSpeed >> junk >> maxRandHealth >> junk;
    P2random::initialize(randomSeed, maxRandDistance, maxRandSpeed, maxRandHealth);
}

void Game::printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m| -s <N> | -v]\n";
} // printHelp()

void Game::createZombie(const string &name, uint32_t distance, uint32_t speed, uint32_t health) {
    Zombie zombie (name, distance, speed, health);
    
    zombieDeq.push_back(zombie);
    zombiePQ.push(&zombieDeq.back());
    if (mode.vMode) {
        cout << "Created: " << name << " " << "(distance: " << distance << ", speed: " << speed 
                << ", health: " << health << ")\n";
    }
}

void Game::readRound() {
    string junk;
    cin >>junk >> zombieRound;
}

void Game::readNewZombie() {
    uint32_t randomZombie = 0;
    uint32_t namedZombie = 0;
    string junk;

    cin >> junk >> randomZombie >> junk >> namedZombie;

    string name;
    uint32_t distance;
    uint32_t speed;
    uint32_t health;

    for (uint32_t i = 1; i <= randomZombie; ++i) {
        name = P2random::getNextZombieName();
        distance = P2random::getNextZombieDistance();
        speed = P2random::getNextZombieSpeed();
        health = P2random::getNextZombieHealth();
        createZombie(name, distance, speed, health);
    }
    for (uint32_t i = 1; i <= namedZombie; ++i) {
        cin >> name >> junk >> distance;
        cin >> junk >> speed;          
        cin >> junk >> health;            
        createZombie(name, distance, speed, health);
    }
    futureRound = !(cin >> junk)? false : true;
}

void Game::playerAttack() {
    while (player.quiver_capacity != 0 && !zombiePQ.empty()) {
        Zombie* curZombie = zombiePQ.top();
        zombiePQ.pop();
        if (player.quiver_capacity < curZombie->health) {
            curZombie->health -= player.quiver_capacity;
            player.quiver_capacity = 0;
            zombiePQ.push(curZombie);

        } else {
            player.quiver_capacity -= curZombie->health;
            curZombie->health = 0;
            deadDeq.push_back(curZombie);
            updateMedian(curZombie->roundsAlive);
            if (mode.vMode) {
                cout << "Destroyed: " << curZombie->name << " (distance: " << curZombie->distance
                    << ", speed: " << curZombie->speed << ", health: " << curZombie->health << ")\n";
            }
            lastEnemy = curZombie->name;
        }
    }
}


uint32_t Game::getMedian() {
    if (first.size() == second.size() + 1) {
        return first.top();
    } else if (first.size() + 1 == second.size()) {
        return second.top();
    }
    return (first.top() + second.top()) / 2;
}

void Game::updateMedian(uint32_t roundsAlive) {
    if (first.empty()) {
        first.push(roundsAlive);
    } else if (second.empty()) {
        second.push(roundsAlive);
    } else {
        if (roundsAlive > first.top()) {
        second.push(roundsAlive);
    } else {
        first.push(roundsAlive);
        }
    }

    if (first.size() >= second.size() + 2) {
        second.push(first.top());
        first.pop();
    } else if (first.size() + 2 <= second.size()) {
        first.push(second.top());
        second.pop();
    }
}

void Game::printStats() {
    cout << "Zombies still active: " << zombieDeq.size() - deadDeq.size() << "\n";
    
    size_t killedN = min(deadDeq.size(), static_cast<size_t>(mode.sModeN));
    cout << "First zombies killed:\n";
    
    for (size_t i = 0; i < killedN; ++i)  {
        cout << deadDeq[i]->name << " " << i+1 <<"\n";
    }
    cout << "Last zombies killed:\n";
    for (size_t i = 0; i < killedN; ++i) {
        cout << deadDeq[deadDeq.size() - i - 1]->name << " " << killedN - i << "\n";
    }

    size_t activeN = min(zombieDeq.size(), static_cast<size_t>(mode.sModeN));
    cout << "Most active zombies:\n";
    sort(zombieDeq.begin(), zombieDeq.end(), MostActive());

    for (size_t i = 0; i < activeN; ++i) {
        cout << zombieDeq[i].name << " " << zombieDeq[i].roundsAlive << "\n";
    }
    cout << "Least active zombies:\n";
    sort(zombieDeq.begin(), zombieDeq.end(), SwapName());

    for (size_t i = 0; i < activeN; ++i) {
        cout << zombieDeq[zombieDeq.size() - i - 1].name << " " << zombieDeq[zombieDeq.size() - i - 1].roundsAlive << "\n";
    }
}

void Game::test01() {
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


int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Game g;
    g.getMode(argc, argv);
    g.test01();

    return 0;
}