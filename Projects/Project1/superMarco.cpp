// B99292359FFD910ED13A7E6C7F9705B8742F0D79
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

    bool operator==(const Location &loc) {
        return loc.room == room && loc.row == row && loc.col == col;
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
    uint32_t tileDiscovered;
    Location start;
    Location end;
    string outputMode;

public:
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

    void solve() {
        deque<Location> locDeque;
        tileDiscovered = 0;
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

            char value = tileVec[next.room][next.row][next.col].value;
            Location temp = next;
            if (isdigit(value)) {
                uint32_t pipeNum = static_cast<uint32_t>(value) - static_cast<uint32_t>('0');

                temp.set(pipeNum, next.row, next.col);
                if (checkAndPush(temp, locDeque, next)) {
                    break;
                };
                continue;
            }
            
            // N
            temp.set(next.room, next.row - 1, next.col);
            if (checkAndPush(temp, locDeque, next)) {
                break;
            }
            // E
            temp.set(next.room, next.row, next.col + 1);
            if (checkAndPush(temp, locDeque, next)) {
                break;
            }
            // S
            temp.set(next.room, next.row + 1, next.col);
            if (checkAndPush(temp, locDeque, next)) {
                break;
            }
            // W
            temp.set(next.room, next.row, next.col - 1);
            if (checkAndPush(temp, locDeque, next)) {
                break;
            }
        }
    } // solve()

    void getOutput() {
        if (!tileVec[end.room][end.row][end.col].isVisited) {
            cout << "No solution, " << tileDiscovered << " tiles discovered.\n";
            return;
        }

        deque<char> backDeq;
        Location current = end;
        Location previous= end;

        while (!(current == start)) {
            previous = tileVec[current.room][current.row][current.col].prev;
            if (current.room != previous.room) {
                backDeq.push_back('p');
            } else if (current.row == previous.row + 1) {
                backDeq.push_back('s');
            } else if (current.row == previous.row - 1) {
                backDeq.push_back('n');
            } else if (current.col == previous.col + 1) {
                backDeq.push_back('e');
            } else {
                backDeq.push_back('w');
            }
            current = previous;
        } // while

        outputMode == "M" ? outputM(backDeq) : outputL(backDeq);
    } // getOutput()

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

            if (value == 'C') {
                end.set(roomN, rowN, colN);
            } else if (value == 'S') {
                start.set(roomN, rowN, colN);
            }
        }
    } // loadL()

    void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-s|q -o M|L\n";
    cout << "This program is to help you learn command-line processing,\n";
    cout << "reading data into a vector, the difference between resize and reserve,\n";
    cout << "and how to properly read until end-of-file.\n";
    } // printHelp()

    bool checkAndPush(const Location &loc, deque<Location> &locDqueue, const Location &prev) {
        if (loc.room <= R && loc.row <= N && loc.col <= N) {
            Tile &curTile = tileVec[loc.room][loc.row][loc.col];
            if (curTile.value == 'C') {
                curTile.isVisited = true;
                return true;
            } // if we find the end
            if (!curTile.isVisited && curTile.value != '#' && curTile.value != '!') {
                if (isdigit(curTile.value)) {
                    uint32_t pipeNum = static_cast<uint32_t>(curTile.value) - static_cast<uint32_t>('0');
                    if (pipeNum > R) {
                        return false;
                    } // if the pipe num is larger than room num
                }
                locDqueue.push_back(loc);
                curTile.isVisited = true;
                curTile.prev = prev;
                ++tileDiscovered;   // add to search container successfully
            }
        }

        return false;
    } // checkAndPush()

    void outputM(deque<char> &backDeq) {
        Location current = start;
        while (!backDeq.empty()) {
            if (backDeq.back() == 'n') {
                tileVec[current.room][current.row][current.col].value = 'n';
                --current.row; 
            } else if (backDeq.back() == 's') {
                tileVec[current.room][current.row][current.col].value = 's';
                ++current.row;
            } else if (backDeq.back() == 'e') {
                tileVec[current.room][current.row][current.col].value = 'e';
                ++current.col;
            } else if (backDeq.back() == 'w') {
                tileVec[current.room][current.row][current.col].value = 'w';
                --current.col;
            } else {
                tileVec[current.room][current.row][current.col].value = 'p';
                current.room = static_cast<uint32_t>(backDeq.back()) - static_cast<uint32_t>('0');
            }
            backDeq.pop_back();
        }

        cout << "Start in room " << start.room << ", row " << start.row << ", column " << start.col << "\n";

        for (uint32_t i =0; i < R; ++i) {
            cout << "//castle room " << i << "\n";
            for (uint32_t j = 0; j < N; ++j) {
                for (uint32_t k = 0; k < N; ++k) {
                    cout << tileVec[i][j][k].value;
                } // for k
                cout << "\n";
            } // for j
        } // for i
    } // outputM()

    void outputL(deque<char> &backDeq) {
        cout << "Path taken:\n";
        Location current = start;
        while (!backDeq.empty()) {
            cout << '(' << current.room << ',' << current.row << ',' << current.col << ',' 
            << backDeq.back() << ")\n";

            if (backDeq.back() == 'n') {
                --current.row; 
            } else if (backDeq.back() == 's') {
                ++current.row;
            } else if (backDeq.back() == 'e') {
                ++current.col;
            } else if (backDeq.back() == 'w') {
                --current.col;
            } else {
                current.room = static_cast<uint32_t>(backDeq.back()) - static_cast<uint32_t>('0');
            }
            backDeq.pop_back();
        } // while
    } //outputL()
};

int main(int argc, char* argv[]) {
    // ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Solver s;
    
    s.getMode(argc, argv);
    s.readMap();
    s.solve();
    s.getOutput();

    return 0;
}