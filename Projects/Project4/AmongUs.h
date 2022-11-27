// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#pragma once
#include <iostream>
#include <limits>
#include <vector>
#include <cmath>

using namespace std;

struct PrimInfo {
    double distance = numeric_limits<double>::infinity();
    uint32_t parent;
    bool isVisitied = false;
};

struct RoomInfo {
    int x;
    int y;
    char type;

    RoomInfo(int xCord, int yCord, char roomType): x{xCord}, y{yCord}, type{roomType} {}
};

class AmongUs {
private:
    vector<PrimInfo> primV;
    vector<RoomInfo> roomV;
    vector<uint32_t> path;
    double bestSoFar;
    uint32_t roomN;
    
public:
    char mode;
    void getMode(int argc, char* argv[]);
    void readFile();
    void mst();
    void fastTsp();
    void optTsp();


private:
    void printHelp(char *argv[]) {
        cout << "Usage: " << argv[0] << " -m, –mode {MST|FASTTSP|OPTTSP}\n";
    }

    double getDistance(uint32_t idx1, uint32_t idx2) {
        double xdiff = static_cast<double>(roomV[idx1].x - roomV[idx2].x);
        double ydiff = static_cast<double>(roomV[idx1].y - roomV[idx2].y);
        return sqrt(xdiff * xdiff + ydiff * ydiff);
    }

    void promising() {

    }

    void solution() {

    }

    void genPerms(size_t permLength) {
        if (permLength == path.size()) {
            if ()
            return;
        }  // if ..complete path

        if (!promising(path, permLength)) {
            return;
        }  // if ..not promising

        for (size_t i = permLength; i < path.size(); ++i) {
            swap(path[permLength], path[i]);
            genPerms(path, permLength + 1);
            swap(path[permLength], path[i]);
        }  // for ..unpermuted elements
    }  // genPerms()
};