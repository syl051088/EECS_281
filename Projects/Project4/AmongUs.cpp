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
    vector<PrimInfo> primV;
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
            if (!primV[j].isVisitied) {
                double newDis = getDistance(roomV[roomIdx], roomV[j]);
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
    } else {

    }

    return 0;
}