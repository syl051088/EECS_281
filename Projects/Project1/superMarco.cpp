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
    bool isVisited = false;
    char value = 'c';
};

struct Location {
public:
    uint32_t room;
    uint32_t row;
    uint32_t col;
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
    // Solver(uint32_t room, uint32_t row, uint32_t col) {
    // }

    void getMode(int argc, char * argv[]) {
    bool modeSpecified = false;
    string mode;

    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "queue", no_argument, nullptr, 'q' },
        { "stack", no_argument, nullptr, 's' },
        { "help", no_argument, nullptr, 'h' },
        { "output", required_argument, nullptr, 'o'},
        { nullptr, 0,                 nullptr, '\0' }
    };

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "qsho:", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);
        case 'q':
            mode = optarg;
        case 's':
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
            load_m();
        } else if (mapMode == 'L') {
            load_l();
        }
    }

    void solve(bool stackMode) {
        deque<Location> tileDeque;
        while (!tileDeque.empty()) {
            if (stackMode) {
            // tileDeque.push_back(mapVec);
            } else {

            }
        }
    }

private:
    void load_m() {
        cin >> R;
        cin >> N;
    }

    void load_l() {
        cin >> R;
        cin >> N;
    }

    void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
    } // printHelp()
};

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Solver s;
    
    s.getMode();
    s.readMap();

    s.solve();

    cout << "Mode is: " << s.mode << "\n";
    cout << "Number of rooms: " << R << "\n";
    cout << "Size of room: " << N << "\n";
    return 0;
}