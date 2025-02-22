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
    char direction;
    char value;
    bool isVisited;

public:
    Tile() {
        isVisited = false;
        value = '.';
    };
};

class Solver {
private:
    vector<vector<vector<Tile>>> tileVec;
    Location start;
    Location end;
    uint32_t R;
    uint32_t N;
    uint32_t tileDiscovered;
    string outputMode;
    bool stackMode;

public:
    Solver () {
        tileDiscovered = 0;
    }

    void getMode(int argc, char * argv[]) {
        uint32_t modeN = 0;
        outputMode = "M";

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
                        cerr << "Output mode must be specified" << endl;
                        exit(1);
                    } else if (outputMode != "M" && outputMode != "L") {
                        cerr << "Unknown command line option" << endl;
                        exit(1);
                    }
                    break;
                
                default:
                    cerr << "Unknown command line option" << endl;
                    exit(1);
            } // switch

            if (modeN == 0) {
                cerr << "Stack or queue must be specified" << endl;
                exit(1);
            } else if (modeN > 1) {
                cerr << "Stack or queue can only be specified once" << endl;
                exit(1);
            }
        } // while
    } // getMode()

    void readMap() {
        string mapMode;
        cin >> mapMode;

        if (mapMode != "M" && mapMode != "L") {
            cerr << "Invalid input mode" << endl;
            exit(1);
        }

        cin >> R;
        if (R < 1 || R > 10) {
            cerr << "Invalid room number" << endl;
            exit(1);
        }
        cin >> N;
        if (N < 1) {
            cerr << "Invalid row number" << endl;
            exit(1);
        }
        
        tileVec.resize(R, vector<vector<Tile>>(N, vector<Tile>(N)));

        mapMode == "M"? loadM() : loadL();
    } //readMap()

    void solve() {
        deque<Location> locDeque;
        Location next = start;
        locDeque.push_back(next);
        ++tileDiscovered;
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
                temp.room = pipeNum;
                if (checkAndPush(temp, locDeque, static_cast<char>(next.room +'0'))) {
                    break;
                };
                continue;
            }

            // N
            temp.set(next.room, next.row - 1, next.col);
            if (checkAndPush(temp, locDeque, 'n')) {
                break;
            }
            // E
            temp.set(next.room, next.row, next.col + 1);
            if (checkAndPush(temp, locDeque, 'e')) {
                break;
            }
            // S
            temp.set(next.room, next.row + 1, next.col);
            if (checkAndPush(temp, locDeque, 's')) {
                break;
            }
            // W
            temp.set(next.room, next.row, next.col - 1);
            if (checkAndPush(temp, locDeque, 'w')) {
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
        char direction;

        while (!(current == start)) {
            direction = tileVec[current.room][current.row][current.col].direction;

            if (direction == 'n') {
                current.row++;
                backDeq.push_back('n');
            } else if (direction == 'e') {
                current.col--;
                backDeq.push_back('e');
            } else if (direction == 's') {
                current.row--;
                backDeq.push_back('s');
            } else if (direction == 'w') {
                current.col++;
                backDeq.push_back('w');
            } else {
                backDeq.push_back(static_cast<char>(current.room + static_cast<uint32_t>('0')));
                current.room = static_cast<uint32_t>(direction) - static_cast<uint32_t>('0');
            }
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
                cerr << "Unknown map character" << endl;
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
                cerr << "Invalid room number" << endl;
                exit(1);
            } else if (rowN >= N) {
                cerr << "Invalid row number" << endl;
                exit(1);
            } else if (colN >= N) {
                cerr << "Invalid col number" << endl;
                exit(1);
            } else if (!isdigit(value) && value != 'C' && value != 'S' && value != '#' && value != '!' && value !='.') {
                cerr << "Unknown map character" << endl;
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
    cout << "Usage: " << argv[0] << " [-s|q -o M|L]\n";
    } // printHelp()

    bool checkAndPush(const Location &loc, deque<Location> &locDqueue, char direction) {
        if (loc.room < R && loc.row < N && loc.col < N) {
            Tile &curTile = tileVec[loc.room][loc.row][loc.col];
            
            if (!curTile.isVisited && curTile.value != '#' && curTile.value != '!') {
                locDqueue.push_back(loc);
                ++tileDiscovered;
                curTile.isVisited = true;
                curTile.direction = direction; // add to search container successfully

                if (curTile.value == 'C') {
                    return true;
                } // if we find the end
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
            cout << '(' << current.room << ',' << current.row << ',' << current.col << ',';

            if (backDeq.back() == 'n') {
                cout << backDeq.back() << ")\n";
                --current.row; 
            } else if (backDeq.back() == 's') {
                cout << backDeq.back() << ")\n";
                ++current.row;
            } else if (backDeq.back() == 'e') {
                cout << backDeq.back() << ")\n";
                ++current.col;
            } else if (backDeq.back() == 'w') {
                cout << backDeq.back() << ")\n";
                --current.col;
            } else {
                current.room = static_cast<uint32_t>(backDeq.back()) - static_cast<uint32_t>('0');
                cout << "p)\n";
            }
            backDeq.pop_back();
        } // while
    } //outputL()
};

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    Solver s;
    
    s.getMode(argc, argv);
    s.readMap();
    s.solve();
    s.getOutput();
    return 0;
}