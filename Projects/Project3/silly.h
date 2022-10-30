// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include "table.h"
#include <string>

using namespace std;

class SQL {
private:
    map<string, Table> m;
    bool qMode = false;
public:
    void getMode(int argc, char *argv[]);
    void create();
    void remove();

private:
    void printHelp(char *argv[]) {
        cout << "Usage: " << argv[0] << " [-q| -quiet]\n";
    } // printHelp()
};