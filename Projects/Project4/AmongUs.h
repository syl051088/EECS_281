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

    RoomInfo(int xCord, int yCord): x{xCord}, y{yCord} {
        if (x > 0 || y > 0) {
            type = 'o';
        } else if (x < 0 && y < 0) {
            type = 'l';
        } else {
            type = 'd';
        }
    }
};

class AmongUs {
private:
    vector<PrimInfo> primV;
    vector<RoomInfo> roomV;
    vector<uint32_t> path;
    vector<uint32_t> bestPath;
    vector<vector<double>> disMatrix;
    double bestDis = 0;
    double runningTotal = 0;
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
        double xdiff = static_cast<double>(roomV[idx1].x) - static_cast<double>(roomV[idx2].x);
        double ydiff = static_cast<double>(roomV[idx1].y) - static_cast<double>(roomV[idx2].y);
        return sqrt(xdiff * xdiff + ydiff * ydiff);
    }

    bool promising(size_t permLength) {
        if (roomN - static_cast<uint32_t>(permLength) < 6) return true;

        double mstCost = localMst(permLength);
        double arm1Len = numeric_limits<double>::infinity();
        double arm2Len = numeric_limits<double>::infinity();
        for (size_t i = permLength; i < path.size(); ++i) {
            if (disMatrix[path[i]][path[0]] < arm1Len)
                arm1Len = disMatrix[path[i]][path[0]];
            if (disMatrix[path[i]][path[permLength - 1]] < arm2Len) 
                arm2Len = disMatrix[path[i]][path[permLength - 1]];
        }
        return mstCost + runningTotal + arm1Len + arm2Len < bestDis;
    }

    void genPerms(size_t permLength) {
        if (permLength == path.size()) {
            runningTotal += disMatrix[path[0]][path.back()];
            if (runningTotal < bestDis) {
                bestDis = runningTotal;
                bestPath = path;
            }
            runningTotal -= disMatrix[path[0]][path.back()];
            return;
        }

        if (!promising(permLength))
            return;

        for (size_t i = permLength; i < path.size(); ++i) {
            swap(path[permLength], path[i]);
            runningTotal += disMatrix[path[permLength]][path[permLength - 1]];
            genPerms(permLength + 1);
            runningTotal -= disMatrix[path[permLength]][path[permLength - 1]];
            swap(path[permLength], path[i]);
        }  // for ..unpermuted elements
    }  // genPerms()

    double localMst(size_t permLength) {
        primV.clear();
        primV.resize(roomN);
        for (size_t i = 0; i < permLength; ++i)
            primV[path[i]].isVisitied = true;

        primV[path[permLength]].distance = 0;
        double mstCost = 0;
        for (uint32_t i = static_cast<uint32_t>(permLength); i < roomN; ++i) {
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
            mstCost += primV[roomIdx].distance;
            for (uint32_t j = 0; j < roomN; ++j) {
                if (!primV[j].isVisitied) {
                    double newDis = disMatrix[roomIdx][j];
                    if (newDis < primV[j].distance) {
                        primV[j].distance = newDis;
                    }
                }
            }
        }
        return mstCost;
    }
};