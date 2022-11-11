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

Table::Table(vector<EntryType> v, vector<string> m, string n): colType{v}, colName{m}, tableName{n}, col{static_cast<uint32_t>(v.size())}{}

void Table::insert(uint32_t N) {
    data.resize(data.size() + N);
    for (uint32_t i = row; i < N + row; ++i) {
        auto & currentRow = data[i];
        currentRow.reserve(col);
        for (uint32_t j = 0; j < col; ++j) {
            switch (colType[j]){
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
    }
    
    if (indexType == "hash") {
        for (uint32_t i = row; i < N + row; ++i) {
            hash[data[i][static_cast<uint32_t>(genIndex)]].push_back(i);
        }
        
    } else if (indexType == "bst") {
        for (uint32_t i = row; i < N + row; ++i) {
            bst[data[i][static_cast<uint32_t>(genIndex)]].push_back(i);
        }
    }
    row += N;
}

void Table::printAll(const vector<uint32_t> &v, bool quiet) {
    if (!quiet) {
        for (uint32_t i : v) {
            cout << colName[i] << ' ';
        }
        cout << '\n';

        for (uint32_t i = 0; i < row; ++i) {
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

void Table::printWhere(const vector<uint32_t> &v, bool quiet) {
    string name;
    char OP;
    cin >> name >> OP;
    int index = findCol(name);
    if (index == -1) {
        cout << "Error during PRINT: " << name << " does not name a column in "<< tableName << '\n';
        getline(cin, name);
        return;
    }
    
    if (!quiet) {
        for (uint32_t i : v) {
            cout << colName[i] << ' ';
        }
        cout << '\n';
    }
    size_t N;
    switch (colType[static_cast<uint32_t>(index)]) {
        
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

    uint32_t N;
    switch (colType[static_cast<uint32_t>(index)]) {
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
        for (uint32_t i = 0; i < row; ++i) {
            hash[data[i][static_cast<uint32_t>(genIndex)]].push_back(i);
        }
    } else if (indexType == "bst") {
        bst.clear();
        for (uint32_t i = 0; i < row; ++i) {
            bst[data[i][static_cast<uint32_t>(genIndex)]].push_back(i);
        }
    }
}

uint32_t Table::join(const vector<vector<TableEntry>> & t1Data, uint32_t index1, const vector<printCol>& printV, uint32_t index2, bool quiet) {
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
        unordered_map<TableEntry, vector<uint32_t>> temp;
        for (uint32_t i = 0; i < row; ++i) {
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

    return static_cast<uint32_t>(N);
}

void Table::generate(string idxType, uint32_t idx) {
    indexType = idxType;
    genIndex = static_cast<int>(idx);
    bst.clear();
    hash.clear();
    if (indexType == "hash") {
        for (uint32_t i = 0; i < row; ++i) {
            hash[data[i][idx]].push_back(i);
        }
    } else {
        for (uint32_t i = 0; i < row; ++i) {
            bst[data[i][idx]].push_back(i);
        }
    }

    cout << "Created " << indexType << " index for table " << tableName << " on column " << colName[idx] << '\n';
}
