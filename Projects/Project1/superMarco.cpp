#include <iostream>
#include <deque>
#include <string>
#include <getopt.h>
#include <vector>
#include "xcode_redirect.hpp"
#include "solver.h"
using namespace std;

struct Tile {
public:
    bool isVisited;
    char value;

public:
    Tile() {
        isVisited = false;
        value = '.';
    };
};

struct Location {
public:
    uint32_t room;
    uint32_t row;
    uint32_t col;

public:
    void set(uint32_t room, uint32_t row, uint32_t col) {
        this->room = room;
        this->row = row;
        this->col = col;
    }
};

class Solver{
private:
    vector<vector<vector<Tile>>> mapVec;
    bool stackMode;
    uint32_t R;
    uint32_t N;
    Location start;
    Location end;

public:
    Solver() {
    
    }

    void getMode(int argc, char * argv[]) {
        bool modeSpecified = false;
        string mode;

        // These are used with getopt_long()
        opterr = false; // Let us handle all error output for command line options
        int choice;
        int option_index = 0;
        option long_options[] = {
            { "queue",  no_argument,       nullptr, 'q' },
            { "stack",  no_argument,       nullptr, 's' },
            { "help",   no_argument,       nullptr, 'h' },
            { "output", required_argument, nullptr, 'o' },
            { nullptr,  0,                 nullptr, '\0'}
        };

        // Fill in the double quotes, to match the mode and help options.
        while ((choice = getopt_long(argc, argv, "qsho:", long_options, &option_index)) != -1) {
            switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'q':
                stackMode = false;
                mode = optarg;
            case 's':
                stackMode = true;
                mode = optarg;
                if (mode != "resize" && mode != "reserve" && mode != "nosize") {
                    // The first line of error output has to be a 'fixed' message for the autograder
                    // to show it to you.
                    cerr << "Error: invalid mode" << "\n";
                    // The second line can provide more information, but you won't see it on the AG.
                    cerr << "  I don't know recognize: " << mode << "\n";
                    exit(1);
                } // if
                modeSpecified = true;
                break;
            case 'o':
                mode = 'o';
            } // switch
        } // while

        if (!modeSpecified) {
            cerr << "Error: no mode specified" << endl;
            exit(1);
        } // if
    } // getMode()

    void readMap() {
        char mapMode;
        cin >> mapMode;

        if (mapMode == 'M') {
            loadM();
        } else if (mapMode == 'L') {
            loadL();
        }
    } //readMap()

    void solve(bool stackMode) {
        deque<Location> locDeque;
        Location next = start;
        locDeque.push_back(next);
        mapVec[next.room][next.row][next.col].isVisited = true;

        while (!locDeque.empty()) {
            if (stackMode) {
                next = locDeque.back();
                locDeque.pop_back();
            } else {
                next = locDeque.front();
                locDeque.pop_front();
            }

            char c = mapVec[next.room][next.row][next.col].value;
            if (isdigit(c)) {
                int pipeNum = static_cast<uint32_t>(c);

                if (pipeNum <= R) {
                    next.set(pipeNum, next.row, next.col);
                }
            }
            // N
            next.set(next.room, next.row - 1, next.col);
            checkAndPush(next, locDeque);
            // E
            next.set(next.room, next.row, next.col + 1);
            checkAndPush(next, locDeque);
            // S
            next.set(next.room, next.row + 1, next.col);
            checkAndPush(next, locDeque);
            // W
            next.set(next.room, next.row, next.col - 1);
            checkAndPush(next, locDeque);
        }
    } // solve()

    void getOutput() {

    } // getOutput()

private:
    void loadM() {
        cin >> R;
        cin >> N;

        mapVec.resize(R, vector<vector<Tile>>(N, vector<Tile>(N)));
        char c;
        string junk;

        while (cin >> c) {
            if (c == '/') {
                getline(cin, junk);
            }

            cin >> N >> junk;
        }
    } // loadM()

    void loadL() {
        cin >> R;
        cin >> N;
    } // loadL()

    void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
    } // printHelp()

    bool checkAndPush(const Location &loc, deque<Location> &locDqueue) {
        if (loc.room <= R && loc.row <= N && loc.col <= N) {
            Tile &curTile = mapVec[loc.room][loc.col][loc.row];
            if (!curTile.isVisited && curTile.value != '#' && curTile.value != '!') {
                locDqueue.push_back(loc);
                curTile.isVisited = true;
            }
        }
        
    } // checkAndPush()

    void outputM() {

    } // outputM

    void outputL() {

    } //outputL
};

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Solver s;
    
    s.getMode(argc, argv);
    s.readMap();

    s.solve(false);

    s.getOutput();

    return 0;
}