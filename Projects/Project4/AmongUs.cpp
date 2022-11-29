// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#include "AmongUs.h"
#include "xcode_redirect.hpp"
#include <getopt.h>
#include <iomanip>

using namespace std;

void AmongUs::getMode(int argc, char* argv[]) {
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    string modeArg = "";
    option long_options[] = {
        { "mode", required_argument, nullptr, 'm' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr,  0,                 nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'm':
                modeArg = std::string(optarg);
                if (modeArg == "MST") {
                    mode = 'm';
                } else if (modeArg == "FASTTSP") {
                    mode = 'f';
                } else if (modeArg == "OPTTSP") {
                    mode = 'o';
                } else {
                    modeArg.size() == 0 ? cerr << "Error: No mode specified" << endl : cerr << "Error: Invalid mode" << endl;
                    exit(1);
                }
                break;
            default:
                cerr << "Error: Invalid command line option" << endl;
                exit(1);
        } // switch
    } // while
}

void AmongUs::readFile() {
    cin >> roomN;
    roomV.reserve(roomN);
    int x, y;
    for (uint32_t i = 0; i < roomN; ++i) {
        cin >> x >> y;
        roomV.emplace_back(x, y);
    }
}

void AmongUs::mst() {
    primV.resize(roomN);
    primV[0].distance = 0;
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
        if (curMinDis == numeric_limits<double>::infinity()) {
             cerr << "Cannot construct MST" << endl;
            exit(1);          
        }
        primV[roomIdx].isVisitied = true;
        runningTotal += curMinDis;
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
    cout << runningTotal << '\n';
    for (uint32_t i = 1; i < roomN; ++i)
        i < primV[i].parent ? cout << i << ' ' << primV[i].parent << '\n' : cout << primV[i].parent << ' ' << i << '\n';
}

void AmongUs::fastTsp() {
    primV.clear();
    primV.resize(roomN);
    bestPath.reserve(roomN);
    bestPath.push_back(0);
    primV[0].isVisitied = true;
    bestPath.push_back(1);
    primV[1].isVisitied = true;
    bestPath.push_back(2);
    primV[2].isVisitied = true;
    bestDis = getDistance(0, 1) + getDistance(1, 2) + getDistance(0, 2);
    for (uint32_t i = 3; i < roomN; ++i) {
        double minDis = numeric_limits<double>::infinity();
        uint32_t tempIdx = 0;
        for (uint32_t j = 0; j < static_cast<uint32_t>(bestPath.size() - 1); ++j) {
            double newDis = getDistance(bestPath[j], i) + getDistance(i, bestPath[j + 1]) - getDistance(bestPath[j], bestPath[j + 1]);
            if (newDis < minDis) {
                minDis = newDis;
                tempIdx = j;
            }
        }
        double lastDis = getDistance(0, i) + getDistance(i, bestPath.back()) - getDistance(0, bestPath.back());
        if (lastDis < minDis) {
            minDis = lastDis;
            bestPath.push_back(i);
        } else {
            bestPath.insert(bestPath.begin() + tempIdx + 1, i);
        }
        primV[i].isVisitied = true;
        bestDis += minDis;
    }

    if (mode == 'f') {
        cout << bestDis << '\n';
        for (uint32_t i = 0; i < roomN; ++i)
            cout << bestPath[i] << ' ';
        cout << '\n';
    }
}

void AmongUs::optTsp() {
    fastTsp();
    path = bestPath;
    disMatrix.resize(roomN, vector<double>(roomN));
    for (uint32_t i = 0; i < roomN; ++i) {
        for (uint32_t j = i; j < roomN; ++j) {
            double dis = getDistance(i, j);
            disMatrix[i][j] = dis;
            disMatrix[j][i] = dis;
        }
    }
    genPerms(1);
    cout << bestDis << '\n';
    for (uint32_t i = 0; i < roomN; ++i)
        cout << bestPath[i] << ' ';
    cout << '\n';
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc,argv);
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
        au.optTsp();
    }

    return 0;
}