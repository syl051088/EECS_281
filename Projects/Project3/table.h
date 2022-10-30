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
    vector<EntryType> colType;
    vector<string> colName;
    vector<vector<TableEntry>> data;
    size_t row = 0;
    size_t col = 0;
public:
    Table(){}
    Table(string n, vector<EntryType> v, vector<string> m);

    void expend(size_t N) {
        data.reserve(data.size() + N);
    }

    size_t getNCol() {
        return col;
    }

    size_t getNRow() {
        return row;
    }

    void insert();
    void printAll(vector<size_t> &v, bool quiet);
    void printCondition();
    int findCol(string colName);

};