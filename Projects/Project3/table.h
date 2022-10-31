// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#pragma once
#include <iostream>
#include "TableEntry.h"
#include <unordered_map>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct GreaterThan {
    TableEntry val;
    size_t index;

    GreaterThan(TableEntry value, size_t colIndex): val{value}, index{colIndex}{}
    bool operator()(const vector<TableEntry>  &row) {
        return row[index] > val;
    }
};

struct LessThan {
    TableEntry val;
    size_t index;

    LessThan(TableEntry value, size_t colIndex): val{value}, index{colIndex}{}
    bool operator()(const vector<TableEntry> &row) {
        return row[index] < val;
    }
};

struct EqualTo {
    TableEntry val;
    size_t index;

    EqualTo(TableEntry value, size_t colIndex): val{value}, index{colIndex}{}

    bool operator()(const vector<TableEntry> &row) {
        return row[index] == val;
    }
};

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
    void printAll(const vector<size_t> &v, bool quiet);
    void printWhere();
    void deleteWhere();
    int findCol(string colName);

private:
    template<typename COMP_FUNCTOR>
    void printWhereHelp() {

    }

    uint32_t deleteWhereHelper(char OP, int index, const TableEntry& condition) {
        auto it = data.begin();
        switch (OP) {
        case '>':
            it = remove_if(data.begin(), data.end(), GreaterThan(condition, static_cast<size_t>(index)));
            break;
        case '<':
            it = remove_if(data.begin(), data.end(), LessThan(condition, static_cast<size_t>(index)));
            break;
        case '=': 
            it = remove_if(data.begin(), data.end(), EqualTo(condition, static_cast<size_t>(index)));
            break;
        }
        return static_cast<uint32_t>(it - data.begin());
    }
};

