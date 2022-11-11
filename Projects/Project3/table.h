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
    uint32_t index;

    GreaterThan(TableEntry value, uint32_t colIndex): val{value}, index{colIndex}{}
    bool operator()(const vector<TableEntry>  &row) {
        return row[index] > val;
    }
};

struct LessThan {
    TableEntry val;
    uint32_t index;

    LessThan(TableEntry value, uint32_t colIndex): val{value}, index{colIndex}{}
    bool operator()(const vector<TableEntry> &row) {
        return row[index] < val;
    }
};

struct EqualTo {
    TableEntry val;
    uint32_t index;

    EqualTo(TableEntry value, uint32_t colIndex): val{value}, index{colIndex}{
    }

    bool operator()(const vector<TableEntry> &row) {
        return row[index] == val;
    }
};

struct printCol{
    uint32_t printIndex;
    bool isTable1;

    printCol(uint32_t idx, bool val): printIndex{idx}, isTable1{val} {};
};

class Table {
private:
    unordered_map<TableEntry, vector<uint32_t>> hash;
    map<TableEntry, vector<uint32_t>> bst;
    vector<vector<TableEntry>> data;
    vector<EntryType> colType;
    vector<string> colName;
    string indexType = "NA";
    string tableName;
    uint32_t row = 0;
    uint32_t col = 0;
    int genIndex = -1;
public:
    Table(){}
    Table(vector<EntryType> v, vector<string> m, string n);

    uint32_t getNRow() {
        return row;
    }

    vector<vector<TableEntry>> & getData() {
        return data;
    }

    void insert(uint32_t index);
    void printAll(const vector<uint32_t> &v, bool quiet);
    void printWhere(const vector<uint32_t> &v, bool quiet);
    void deleteWhere(int index);
    uint32_t join(const vector<vector<TableEntry>> & t1Data, uint32_t index1, const vector<printCol>& printV, uint32_t index2, bool quiet);
    void generate(string idxType, uint32_t idx);
    int findCol(string name);

private:
    template<typename COMP_FUNCTOR>
    uint32_t printWhereHelper(bool quiet, const vector<uint32_t> &v, COMP_FUNCTOR comp) {
        uint32_t N = 0;
        for (uint32_t i = 0; i < row; ++i) {
            if (comp(data[i])) {
                ++N;
                if (!quiet) {
                    for (uint32_t j : v) {
                        cout << data[i][j] << ' ';
                    }
                    cout << '\n';
                }
            }
        }

        return N;
    }
    size_t printWhereBst(char OP, const TableEntry& condition, bool quiet, const vector<uint32_t> &v) {
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
                    for (uint32_t j : i->second) {
                        for (uint32_t k : v) {
                            cout << data[j][k] << ' ';
                        }
                        cout << '\n';
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
                    for (uint32_t j : i->second) {
                        for (uint32_t k : v) {
                            cout << data[j][k] << ' ';
                        }
                        cout << '\n';
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
                for (uint32_t i : it->second) {
                    for (uint32_t j : v) {
                        cout << data[i][j] << ' ';
                    }
                    cout << '\n';
                }
            }
            break;
        }
        }
        return N;
    }

    size_t printWhereNormal(char OP, int index, const TableEntry& condition, bool quiet, const vector<uint32_t> &v) {
        size_t N = 0;
        switch (OP) {
        case '>': {
            N = printWhereHelper(quiet, v, GreaterThan(condition, static_cast<uint32_t>(index)));
            break;
        }
        case '<': {
            N = printWhereHelper(quiet, v, LessThan(condition, static_cast<uint32_t>(index)));
            break;
        }
        case '=': {
            if (genIndex == index && indexType == "hash") {
                auto it = hash.find(condition);
                if (it != hash.end()) {
                    N = it->second.size();
                    if (!quiet) {
                        for (uint32_t i : it->second) {
                            for (uint32_t j : v) {
                                cout << data[i][j] << ' ';
                            }
                            cout << '\n';
                        }
                    }  
                }
            } else {
                N = printWhereHelper(quiet, v, EqualTo(condition, static_cast<uint32_t>(index)));
            }
            break;
        } 
        }

        return N;
    }

    uint32_t deleteWhereHelper(char OP, int index, const TableEntry& condition) {
        auto it = data.begin();
        switch (OP) {
        case '>':
            it = remove_if(data.begin(), data.end(), GreaterThan(condition, static_cast<uint32_t>(index)));
            break;
        case '<':
            it = remove_if(data.begin(), data.end(), LessThan(condition, static_cast<uint32_t>(index)));
            break;
        case '=':
            it = remove_if(data.begin(), data.end(), EqualTo(condition, static_cast<uint32_t>(index)));
            break;
        }
        
        uint32_t N = static_cast<uint32_t>(data.end() - it);
        data.erase(it, data.end());
        row -= N;

        return N;
    }
};

