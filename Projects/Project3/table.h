// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#pragma once
#include <iostream>
#include "TableEntry.h"
#include <unordered_map>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Table {
private:
    string tableName;
    vector<EntryType> entryType;
    vector<string> entryName;
public:
    Table(string n, vector<EntryType> v, vector<string> m): tableName{n}, entryType{v}, entryName{m}{

    }

};