// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#pragma once
#include <iostream>
#include <algorithm>
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

struct printCol{
    size_t printIndex;
    bool isTable1;

    printCol(size_t idx, bool val): printIndex{idx}, isTable1{val} {};
};

class Table {
private:
    string tableName;
    unordered_map<TableEntry, vector<size_t>> hash;
    map<TableEntry, vector<size_t>> bst;
    vector<EntryType> colType;
    vector<string> colName;
    vector<vector<TableEntry>> data;
    string indexType = "NA";
    size_t row = 0;
    size_t col = 0;
    int genIndex = -1;
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

    vector<vector<TableEntry>> & getData() {
        return data;
    }

    string getIdxType() {
        return indexType;
    }

    void insert();
    void printAll(const vector<size_t> &v, bool quiet);
    void printWhere(const vector<size_t> &v, bool quiet);
    void deleteWhere(int index);
    size_t join(const vector<TableEntry>& rowV, size_t i, const vector<printCol>& printV, size_t genIdx, bool quiet);
    void generate(string idxType, size_t idx);
    int findCol(string colName);

private:
    template<typename COMP_FUNCTOR>
    size_t printWhereHelper(bool quiet, const vector<size_t> &v, COMP_FUNCTOR comp) {
        size_t N = 0;
        for (size_t i = 0; i < row; ++i) {
            if (comp(data[i])) {
                ++N;
                if (!quiet) {
                    for (size_t j : v) {
                        cout << data[i][j] << ' ';
                    }
                    cout << endl;
                }
            }
        }

        return N;
    }
    size_t printWhereBst(char OP, const TableEntry& condition, bool quiet, const vector<size_t> &v) {
        size_t N = 0;
        auto itBegin = bst.begin();
        auto it = bst.begin();
        auto itEnd = bst.end();
        switch (OP) {
        case '>': {
        it = bst.upper_bound(condition);
            for (auto i = it; i != itEnd; ++i) {
                N += i->second.size();
                if (!quiet) {
                    for (size_t j : i->second) {
                        for (size_t k : v) {
                            cout << data[j][k] << ' ';
                        }
                        cout << endl;
                    }
                } 
            }
            break;
        }
        case '<': {
            it = bst.lower_bound(condition);
            for (auto i = itBegin; i != it; ++i) {
                N += i->second.size();
                if (!quiet) {
                    for (size_t j : i->second) {
                        for (size_t k : v) {
                            cout << data[j][k] << ' ';
                        }
                        cout << endl;
                    }
                }  
            }
            break;
        }
        case '=': {
            it = bst.find(condition);
            
            if (it == bst.end()) {
                return 0;
            }
            N = it->second.size();
            if (!quiet) {
                for (size_t i : it->second) {
                    for (size_t j : v) {
                        cout << data[i][j] << ' ';
                    }
                    cout << endl;
                }
            }
            break;
        }
        }
        return N;
    }

    size_t printWhereNormal(char OP, int index, const TableEntry& condition, bool quiet, const vector<size_t> &v) {
        size_t N = 0;
        switch (OP) {
        case '>': {
            N = printWhereHelper(quiet, v, GreaterThan(condition, static_cast<size_t>(index)));
            break;
        }
        case '<': {
            N = printWhereHelper(quiet, v, LessThan(condition, static_cast<size_t>(index)));
            break;
        }
        case '=': {
            if (genIndex == index && indexType == "hash") {
                auto it = hash.find(condition);
                if (it != hash.end()) {
                    N = it->second.size();
                    if (!quiet) {
                        for (size_t i : it->second) {
                            for (size_t j : v) {
                                cout << data[i][j] << ' ';
                            }
                            cout << endl;
                        }
                    }  
                }
            } else {
                N = printWhereHelper(quiet, v, EqualTo(condition, static_cast<size_t>(index)));
            }
            break;
        } 
        }

        return N;
    }

    size_t deleteWhereHelper(char OP, int index, const TableEntry& condition) {
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
        size_t N = data.size() - static_cast<size_t>(it - data.begin());
        data.erase(it, data.end());
        row = data.size();
        return N;
    }
};

