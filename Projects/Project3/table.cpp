// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <iostream>
#include "TableEntry.h"
#include "table.h"
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Table::Table(string n, vector<EntryType> v, vector<string> m): tableName{n}, colType{v}, colName{m}, col{v.size()}{}

void Table::insert() {
    vector<TableEntry> newData;
    newData.reserve(col);
    for (size_t i = 0; i < col; ++i) {
        switch (colType[i]){
        case EntryType::String: {
            string str;
            cin >> str;
            newData.emplace_back(str);
            break;
        }
        case EntryType::Double: {
            double val;
            cin >> val;
            newData.emplace_back(val);
            break;
        }
        case EntryType::Int: {
            int val;
            cin >> val;
            newData.emplace_back(val);
            break;
        }
        case EntryType::Bool: {
            bool val;
            cin >> val;
            newData.emplace_back(val);
            break;
        }
        }
    }
    data.emplace_back(newData);
    if (indexType == "hash") {
        hash[data[row][static_cast<size_t>(genIndex)]].push_back(row);
    } else if (indexType == "bst") {
        bst[data[row][static_cast<size_t>(genIndex)]].push_back(row);
    }
    ++row;
}

void Table::printAll(const vector<size_t> &v, bool quiet) {
    if (!quiet) {
        for (size_t i : v) {
            cout << colName[i] << ' ';
        }
        cout << endl;

        for (size_t i = 0; i < row; ++i) {
            for (auto index : v) {
                cout << data[i][index] << ' ';
            }
            cout << endl;
        }
    }
    cout << "Printed " << row << " matching rows from " << tableName << endl;
}

int Table::findCol(string name) {
    auto it = find(colName.begin(), colName.end(), name);
    return it != colName.end() ? static_cast<int>(it - colName.begin()) : -1;
}

void Table::printWhere(const vector<size_t> &v, bool quiet) {
    string colNamee;
    char OP;
    cin >> colNamee >> OP;
    int index = findCol(colNamee);
    if (index == -1) {
        cout << "Error during PRINT: " << colNamee << " does not name a column in "<< tableName << endl;
        getline(cin, colNamee);
        return;
    }
    if (!quiet) {
        for (size_t i : v) {
            cout << colName[i] << ' ';
        }
        cout << endl;
    }
    size_t N;
    switch (colType[static_cast<size_t>(index)]) {
        case EntryType::String: {
                string str;
                cin >> str;
                N = (genIndex == index && indexType == "bst") ? printWhereBst(OP, TableEntry{str}, quiet, v) 
                    : printWhereNormal(OP, index, TableEntry{str}, quiet, v);
                break;
            }
            case EntryType::Double: {
                double val;
                cin >> val;
                N = (genIndex == index && indexType == "bst") ? printWhereBst(OP, TableEntry{val}, quiet, v) 
                    : printWhereNormal(OP, index, TableEntry{val}, quiet, v);
                break;
            }
            case EntryType::Int: {
                int val;
                cin >> val;
                N = (genIndex == index && indexType == "bst") ? printWhereBst(OP, TableEntry{val}, quiet, v) 
                    : printWhereNormal(OP, index, TableEntry{val}, quiet, v);
                break;
            }
            case EntryType::Bool: {
                bool val;
                cin >> val;
                N = (genIndex == index && indexType == "bst") ? printWhereBst(OP, TableEntry{val}, quiet, v) 
                    : printWhereNormal(OP, index, TableEntry{val}, quiet, v);
                break;
            }
        }
    cout << "Printed " << N << " matching rows from " << tableName << endl;
}

void Table::deleteWhere(int index) {
    char OP;
    cin >> OP;

    size_t N;
    switch (colType[static_cast<size_t>(index)]) {
    case EntryType::String: {
            string val;
            cin >> val;
            N = deleteWhereHelper(OP, index, TableEntry{val});
            break;
        }
        case EntryType::Double: {
            double val;
            cin >> val;
            N = deleteWhereHelper(OP, index, TableEntry{val});
            break;
        }
        case EntryType::Int: {
            int val;
            cin >> val;
            N = deleteWhereHelper(OP, index, TableEntry{val});
            break;
        }
        case EntryType::Bool: {
            bool val;
            cin >> val;
            N = deleteWhereHelper(OP, index, TableEntry{val});
            break;
        }
    }
    cout << "Deleted " << N << " rows from " << tableName << endl;

    if (indexType == "hash") {
        hash.clear();
        for (size_t i = 0; i < row; ++i) {
            hash[data[i][static_cast<size_t>(genIndex)]].push_back(i);
        }
    } else if (indexType == "bst") {
        bst.clear();
        for (size_t i = 0; i < row; ++i) {
            bst[data[i][static_cast<size_t>(genIndex)]].push_back(i);
        }
    }
}

size_t Table::join(const vector<TableEntry>& rowV, size_t i, const vector<printCol>& printV, size_t genIdx, bool quiet) {
    size_t N = 0;
    const TableEntry& target = rowV[i];
    if (indexType == "hash" && genIndex == static_cast<int>(genIdx)) {
        auto it = hash.find(target);
        if (it != hash.end()) {
            N = it->second.size();
            if (!quiet) {
                for (auto i : it->second) {
                    for (auto j : printV) {
                        if (j.isTable1) {
                            cout << rowV[j.printIndex] << ' ';
                        } else {
                            cout << data[i][j.printIndex] << ' ';
                        }
                    }
                    cout << endl;
                }
            }
        }
    } else if (indexType == "bst" && genIndex == static_cast<int>(genIdx)) {
        auto it = bst.find(target);
        if (it != bst.end()) {
            N = it->second.size();
            if (!quiet) {
                for (auto i : it->second) {
                    for (auto j : printV) {
                        if (j.isTable1) {
                            cout << rowV[j.printIndex] << ' ';
                        } else {
                            cout << data[i][j.printIndex] << ' ';
                        }
                    }
                    cout << endl;
                }
            }
        }
    } else {
        for (size_t i = 0; i < row; ++i) {
            if (data[i][genIdx] == target) {
                ++N;
                if (!quiet) {
                    for (auto j : printV) {
                        if (j.isTable1) {
                            cout << rowV[j.printIndex] << ' ';
                        } else {
                            cout << data[i][j.printIndex] << ' ';
                        }
                    }
                    cout << endl;
                }
            }
        }
    }

    return N;
}

void Table::generate(string idxType, size_t idx) {
    indexType = idxType;
    genIndex = static_cast<int>(idx);
    bst.clear();
    hash.clear();
    if (indexType == "hash") {
        for (size_t i = 0; i < row; ++i) {
            hash[data[i][idx]].push_back(i);
        }
    } else {
        for (size_t i = 0; i < row; ++i) {
            bst[data[i][idx]].push_back(i);
        }
    }

    cout << "Created " << indexType << " index for table " << tableName << " on column " << colName[idx] << endl;
}
