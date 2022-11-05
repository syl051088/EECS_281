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
    ++row;
}

void Table::printAll(const vector<size_t> &v, bool quiet) {
    if (!quiet) {
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

void Table::printWhere() {
    string str;
    char OP;
    cin >> OP >> str;
    int index = findCol(str);
    if (index == -1) {
        cout << "Error during PRINT: " << str << " does not column a table in "<< tableName << '\n';
        getline(cin, str);
        return;
    }
}

void Table::deleteWhere() {
    string str;
    char OP;
    cin >> str >> OP;
    int index = findCol(str);
    if (index == -1) {
        cout << "Error during DELETE: " << str << " does not column a table in "<< tableName << '\n';
        getline(cin, str);
        return;
    }
    size_t N;
    switch (colType[static_cast<size_t>(index)]) {
    case EntryType::String: {
            string str;
            cin >> str;
            N = deleteWhereHelper(OP, index, TableEntry{str});
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
}
