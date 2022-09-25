// 9504853406CBAC39EE89AA3AD238AA12CA198043
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
        >> junk >> maxRandSpeed >> junk >> maxRandHealth;
    P2random::initialize(randomSeed, maxRandDistance, maxRandSpeed, maxRandHealth);
    currentRound = 1;
}

void Game::printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m| -s <N> | -v]\n";
} // printHelp()

void Game::createPlayer() {
    player.quiver_capacity = quiverCapacity;

    
}

void Game::createZombie(const string &name, uint32_t distance, uint32_t speed, uint32_t health) {
    Zombie *zombie = new Zombie(name, distance, speed, health);
    zombiePQ.push(zombie);
    zombieDeq.push_back(zombie);
    if (mode.vMode) {
        cout << "Created: " << name << " " << "(distance: " << distance << ", speed: " << speed 
                << ", health: " << health << ")\n";
    }
}

void Game::readRound() {
    string junk;

    if (!(cin >> junk)) {
        return;
    }
    cin >> junk >> zombieRound;
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
        cin >> name >> junk >> distance >> junk >> speed >> junk >> health;
        createZombie(name, distance, speed, health);
    }
}


uint32_t Game::getMedian() {
    return 0;
}

void Game::medianTracker() {

}




int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Game g;
    g.getMode(argc, argv);
    // g.readHeader();

    // g.readRound();
    g.test01();

    return 0;
}