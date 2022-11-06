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

void Table::insert(size_t index) {
    auto & currentRow = data[index];
    currentRow.reserve(col);
    for (size_t i = 0; i < col; ++i) {
        switch (colType[i]){
        case EntryType::String: {
            string str;
            cin >> str;
            currentRow.emplace_back(str);
            break;
        }
        case EntryType::Double: {
            double val;
            cin >> val;
            currentRow.emplace_back(val);
            break;
        }
        case EntryType::Int: {
            int val;
            cin >> val;
            currentRow.emplace_back(val);
            break;
        }
        case EntryType::Bool: {
            bool val;
            cin >> val;
            currentRow.emplace_back(val);
            break;
        }
        }
    }
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
        cout << '\n';

        for (size_t i = 0; i < row; ++i) {
            for (auto index : v) {
                cout << data[i][index] << ' ';
            }
            cout << '\n';
        }
    }
    cout << "Printed " << row << " matching rows from " << tableName << '\n';
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
        cout << "Error during PRINT: " << colNamee << " does not name a column in "<< tableName << '\n';
        getline(cin, colNamee);
        return;
    }
    if (!quiet) {
        for (size_t i : v) {
            cout << colName[i] << ' ';
        }
        cout << '\n';
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
    cout << "Printed " << N << " matching rows from " << tableName << '\n';
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
    cout << "Deleted " << N << " rows from " << tableName << '\n';

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

size_t Table::join(const vector<vector<TableEntry>> & t1Data, size_t index1, const vector<printCol>& printV, size_t index2, bool quiet) {
    size_t N = 0;
    
    if (indexType == "hash" && genIndex == static_cast<int>(index2)) {
        for (auto& rowV : t1Data) {
            const TableEntry& target = rowV[index1];
            auto it = hash.find(target);
            if (it != hash.end()) {
                N += it->second.size();
                if (!quiet) {
                    for (auto i : it->second) {
                        for (auto j : printV) {
                            if (j.isTable1) {
                                cout << rowV[j.printIndex] << ' ';
                            } else {
                                cout << data[i][j.printIndex] << ' ';
                            }
                        }
                        cout << '\n';
                    }
                }
            } 
        }
    } else {
        unordered_map<TableEntry, vector<size_t>> temp;
        for (size_t i = 0; i < row; ++i) {
            temp[data[i][index2]].push_back(i);
        }

        for (auto& rowV : t1Data) {
            const TableEntry& target = rowV[index1];
            auto it = temp.find(target);
            if (it != temp.end()) {
                N += it->second.size();
                if (!quiet) {
                    for (auto i : it->second) {
                        for (auto j : printV) {
                            if (j.isTable1) {
                                cout << rowV[j.printIndex] << ' ';
                            } else {
                                cout << data[i][j.printIndex] << ' ';
                            }
                        }
                        cout << '\n';
                    }
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

    cout << "Created " << indexType << " index for table " << tableName << " on column " << colName[idx] << '\n';
}
