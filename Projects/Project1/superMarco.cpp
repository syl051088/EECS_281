#include <iostream>
#include <deque>
#include <string>
#include <getopt.h>
#include <vector>
#include "xcode_redirect.hpp"
using namespace std;

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

struct Tile {
public:
    bool isVisited;
    char value;
    Location prev;

public:
    Tile() {
        isVisited = false;
        value = '.';
    };
};

class Solver{
private:
    vector<vector<vector<Tile>>> tileVec;
    bool stackMode;
    uint32_t R;
    uint32_t N;
    Location start;
    Location end;
    string outputMode;

public:
    Solver() {
    
    }

    void getMode(int argc, char * argv[]) {
        uint32_t modeN = 0;

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

        while ((choice = getopt_long(argc, argv, "qsho:", long_options, &option_index)) != -1) {
            switch (choice) {
                case 'h':
                    printHelp(argv);
                    exit(0);
                case 'q':
                    stackMode = false;
                    ++modeN;
                    break;
                case 's':
                    stackMode = true;
                    ++modeN;
                    break;
                case 'o':
                    outputMode = optarg;
                    if (outputMode.length() == 0) {
                        cerr << "Output mode must be specified\n";
                        exit(1);
                    } else if (outputMode != "M" && outputMode != "L") {
                        cerr << "Unknown command line option\n";
                        exit(1);
                    }
                    break;
                
                default:
                    cerr << "Unknown command line option\n";
                    exit(1);
            } // switch

            if (modeN == 0) {
                cerr << "Stack or queue must be specified\n";
                exit(1);
            } else if (modeN > 1) {
                cerr << "Stack or queue can only be specified once\n";
                exit(1);
            }
        } // while
    } // getMode()

    void readMap() {
        string mapMode;
        cin >> mapMode;

        if (mapMode != "M" && mapMode != "L") {
            cerr << "Invalid input mode\n";
            exit(1);
        }

        cin >> R;
        if (R < 1 || R > 10) {
            cerr << "Invalid room number\n";
            exit(1);
        }
        cin >> N;
        if (N < 1) {
            cerr << "Invalid row number\n";
            exit(1);
        }
        
        tileVec.resize(R, vector<vector<Tile>>(N, vector<Tile>(N)));

        mapMode == "M"? loadM() : loadL();
    } //readMap()

    void solve(bool stackMode) {
        deque<Location> locDeque;
        Location next = start;
        locDeque.push_back(next);
        tileVec[next.room][next.row][next.col].isVisited = true;

        while (!locDeque.empty()) {
            if (stackMode) {
                next = locDeque.back();
                locDeque.pop_back();
            } else {
                next = locDeque.front();
                locDeque.pop_front();
            }

            char c = tileVec[next.room][next.row][next.col].value;
            if (isdigit(c)) {
                uint32_t pipeNum = static_cast<uint32_t>(c) - static_cast<uint32_t>('0');

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

    // testing
    void test1() {
        for (uint32_t i = 0; i < R; ++i) {
            cout << "//castle room " << i << "\n";
            for (uint32_t j = 0; j < N; ++j) {
                for (uint32_t k = 0; k < N ;++k) {
                    cout << tileVec[i][j][k].value;
                }
                cout << '\n';
            }
        }
    }

private:
    void loadM() {
        char value;
        string junk;
        uint32_t roomN = 0;
        uint32_t rowN = 0;
        uint32_t colN = 0;

        while (cin >> value) {
            if (value == '/') {
                getline(cin, junk);
                continue;
            } else if ( value == 'C') {
                end.set(roomN, rowN, colN);
            } else if (value == 'S') {
                start.set(roomN, rowN, colN);
            } else if (value != '#' && value != '!' && !isdigit(value) && value != '.') {
                cerr << "Unknown map character\n";
                exit(1);
            }
            tileVec[roomN][rowN][colN].value = value;

            
            if (colN == N - 1) {
                colN = 0;
                if (rowN == N - 1) {
                    rowN = 0;
                    ++roomN;
                } else {
                    ++rowN;
                }
            } else {
                ++colN;
            }
        }
    } // loadM()

    void loadL() {
        char value;
        char junk;
        string junkLine;
        uint32_t roomN;
        uint32_t rowN;
        uint32_t colN;

        while (cin >> junk) {
            if (junk == '/') {
                getline(cin, junkLine);
                continue;
            } // if

            cin >> roomN >> junk >> rowN >> junk >> colN >> junk >> value >> junk;

            if (roomN >= R) {
                cerr << "Invalid room number\n";
                exit(1);
            } else if (rowN >= N) {
                cerr << "Invalid row number\n";
                exit(1);
            } else if (colN >= N) {
                cerr << "Invalid col number\n";
                exit(1);
            } else if (!isdigit(value) && value != 'C' && value != 'S' && value != '#' && value != '!' && value !='.') {
                cerr << "Unknown map character\n";
                exit(1);
            }
            tileVec[roomN][rowN][colN].value = value;
        }
    } // loadL()

    void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
    } // printHelp()

    bool checkAndPush(const Location &loc, deque<Location> &locDqueue) {
        if (loc.room <= R && loc.row <= N && loc.col <= N) {
            Tile &curTile = tileVec[loc.room][loc.col][loc.row];
            if (!curTile.isVisited && curTile.value != '#' && curTile.value != '!') {
                locDqueue.push_back(loc);
                curTile.isVisited = true;

                return true;
            }
        }

        return false;
    } // checkAndPush()

    void outputM() {

    } // outputM

    void outputL() {

    } //outputL
};

int main(int argc, char* argv[]) {
    // ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Solver s;
    
    s.getMode(argc, argv);
    s.readMap();

    // s.solve(false);

    // s.getOutput();
    s.test1();

    return 0;
}