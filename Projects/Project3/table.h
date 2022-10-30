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
    vector<vector<TableEntry>> entries;
    size_t row = 0;
    size_t col = 0;
public:
    Table(string n, vector<EntryType> v, vector<string> m): tableName{n}, entryType{v}, entryName{m}{

    }

    void expend(size_t N) {
        entries.reserve(entries.size() + N);
    };

    size_t getNCol() {
        return col;
    }

    size_t getNRow() {
        return row;
    }

    void insert();

};