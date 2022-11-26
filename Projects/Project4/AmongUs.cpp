// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#include "AmongUs.h"
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

void AmongUs::getMode(int argc, char* argv[]) {
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "mode",   no_argument, nullptr, 'm' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr,  0,                 nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'm':
                if (strcmp(optarg, "MST") == 0) {
                    mode = 'm';
                } else if (strcmp(optarg, "FASTTSP") == 0) {
                    mode = 'f';
                } else if (strcmp(optarg, "OPTTSP") == 0) {
                    mode = 'o';
                } else {
                    if (optarg[0] == '\0') cerr << "Error: No mode specified\n";
                    else cerr << "Error: Invalid mode\n";
                    exit(1);
                }
                break;
            default:
                cerr << "Error: Invalid command line option" << '\n';
                exit(1);
        } // switch
    } // while
}

void AmongUs::readFile() {
    cin >> roomN;
    roomV.reserve(roomN);
    int x, y;
    bool lab = false;
    bool outer = false;
    bool decon = false;
    char type;
    for (uint32_t i = 0; i < roomN; ++i) {
        cin >> x >> y;
        if (x > 0 || y > 0) {
            type = 'o';
            outer = true;
        } else if (x < 0 && y < 0) {
            type = 'l';
            lab = true;
        } else {
            type = 'd';
            decon = true;
        }
        roomV.emplace_back(x, y, type);
    }
    if (lab && outer && !decon) {
        cerr << "Cannot construct MST\n";
        exit(1);            
    }
}

void AmongUs::mst() {
    primV.resize(roomN);
    primV[0].distance = 0;
    double runningTotal = 0;
    for (uint32_t i = 0; i < roomN; ++i) {
        double curMinDis = numeric_limits<double>::infinity();
        uint32_t roomIdx = 0;
        for (uint32_t j = 0; j < roomN; ++j) {
            PrimInfo& curPrim = primV[j];
            if (!curPrim.isVisitied && curPrim.distance < curMinDis) {
                curMinDis = curPrim.distance;
                roomIdx = j;
            }
        }
        primV[roomIdx].isVisitied = true;
        runningTotal += primV[roomIdx].distance;
        for (uint32_t j = 0; j < roomN; ++j) {
            if (!primV[j].isVisitied && !((roomV[roomIdx].type == 'o' && roomV[j].type == 'l') || (roomV[j].type == 'o' && roomV[roomIdx].type == 'l'))) {
                double newDis = getDistance(roomIdx, j);
                if (newDis < primV[j].distance) {
                    primV[j].distance = newDis;
                    primV[j].parent = roomIdx;
                }
            }
        }
    }
    if (mode == 'm') {
        cout << runningTotal << '\n';
        for (uint32_t i = 1; i < roomN; ++i) {
            if (i < primV[i].parent) cout << i << ' ' << primV[i].parent << '\n';
            else cout <<  primV[i].parent << ' ' << i << '\n';
        }
    }
}

void AmongUs::fastTsp() {
    primV.clear();
    primV.resize(roomN);
    path.reserve(roomN);
    uint32_t curIdx = 0;
    double runningTotal = 0;
    // nearest neighbor
    // for (uint32_t i = 0; i < roomN; ++i) {
    //     double minDis = numeric_limits<double>::infinity();
    //     uint32_t tempIdx = 0;
    //     for (uint32_t j = 0; j < roomN; ++j) {
    //         if (!primV[j].isVisitied) {
    //             double newDis = getDistance(curIdx, j);
    //             if (newDis < minDis) {
    //                 minDis = newDis;
    //                 tempIdx = j;
    //             }
    //         }
    //     }
    //     curIdx = tempIdx;
    //     path.push_back(curIdx);
    //     primV[curIdx].isVisitied = true;
    //     runningTotal += minDis;
    // }
    // runningTotal += getDistance(curIdx, 0);

    path.push_back(0);
    primV[0].isVisitied = true;
    path.push_back(1);
    primV[1].isVisitied = true;
    path.push_back(2);
    primV[2].isVisitied = true;
    runningTotal += getDistance(0, 1) + getDistance(1, 2) + getDistance(0, 2);
    // double minDis0 = numeric_limits<double>::infinity();
    // for (uint32_t i = 1; i < roomN; ++i) {
    //     double newDis = getDistance(0, i);
    //     if (newDis < minDis0) {
    //         minDis0 = newDis;
    //         curIdx = i;
    //     }
    // }
    // path.push_back(curIdx);
    // runningTotal += 2 * minDis0;
    // primV[curIdx].isVisitied = true;
    for (uint32_t i = 3; i < roomN; ++i) {
        double minDis = numeric_limits<double>::infinity();
        uint32_t tempIdx = i;
        // for (uint32_t j = 0; j < roomN; ++j) {
        //     if (!primV[j].isVisitied) {
        //         double newDis = getDistance(curIdx, j);
        //         if (newDis < minDis) {
        //             minDis = newDis;
        //             tempIdx = j;
        //         }
        //     }
        // }
        curIdx = tempIdx;
        tempIdx = 0;
        minDis = numeric_limits<double>::infinity();
        for (uint32_t j = 0; j < static_cast<uint32_t>(path.size() - 1); ++j) {
            double newDis = getDistance(path[j], curIdx) + getDistance(curIdx, path[j + 1]) - getDistance(path[j], path[j + 1]);
            if (newDis < minDis) {
                minDis = newDis;
                tempIdx = j;
            }
        }
        double lastDis = getDistance(0, curIdx) + getDistance(curIdx, path.back()) - getDistance(0, path.back());
        if (lastDis < minDis) {
            minDis = lastDis;
            path.push_back(curIdx);
        } else {
            path.insert(path.begin() + tempIdx + 1, curIdx);
        }
        primV[curIdx].isVisitied = true;
        runningTotal += minDis;
    }

    if (mode == 'f') {
        cout << runningTotal << '\n';
        for (uint32_t i = 0; i < roomN; ++i) {
            cout << path[i] << ' ';
        }
        cout << '\n';
    }
}

void AmongUs::optTsp() {

}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers

    AmongUs au;
    au.getMode(argc, argv);
    au.readFile();
    if (au.mode == 'm') {
        au.mst();
    } else if (au.mode == 'f') {
        au.fastTsp();
    } else {
        au.fastTsp();
    }

    return 0;
}