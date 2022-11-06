// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <iostream>
#include <getopt.h>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>
#include "silly.h"
#include "TableEntry.h"
#include "table.h"
#include "xcode_redirect.hpp"

using namespace std;

void SQL::getMode(int argc, char * argv[]) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "quiet",   no_argument, nullptr, 'q' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr,  0,                 nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "qh", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'q':
                qMode = true;
                break;
            default:
                cerr << "Error: unrecognized command" << '\n';
                exit(1);
        } // switch
    } // while
} // getMode

void SQL::create() {
    string tableName;
    cin >> tableName;
    if (m.find(tableName) != m.end()) {
        cout << "Error during CREATE: Cannot create already existing table " << tableName << '\n';
        getline(cin, tableName);
        return;
    }
    uint32_t N;
    cin >> N;
    vector<EntryType> typeV;
    typeV.reserve(N);
    string type;
    for (uint32_t i = 0; i < N; ++i) {
        cin >> type;
        switch (type[0]) {
        case 's':
            typeV.emplace_back(EntryType::String);
            break;
        case 'd':
            typeV.emplace_back(EntryType::Double);
            break;
        case 'i':
            typeV.emplace_back(EntryType::Int);
            break;
        case 'b':
            typeV.emplace_back(EntryType::Bool);
            break;
        }
    }

    cout << "New table " << tableName << " with column(s) ";
    string name;
    vector<string> nameV;
    nameV.reserve(N);
    for (uint32_t i = 0; i < N; ++i) {
        cin >> name;
        nameV.push_back(name);
        cout << name << ' ';
    }
    m.emplace(tableName, Table(tableName, typeV, nameV));
    cout << "created" << '\n';
}

void SQL::remove() {
    string tableName;
    cin >> tableName;
    auto it = m.find(tableName);
    if (it == m.end()) {
        cout << "Error during REMOVE: " << tableName << " does not name a table in the database\n";
        return;
    }
    m.erase(it);
    cout << "Table " << tableName << " deleted\n";
}

void SQL::insert() {
    string tableName, junk;
    uint32_t N;
    cin >> junk >> tableName >> N >> junk;
    if (m.find(tableName) == m.end()) {
        cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";
        getline(cin, junk);
        for (uint32_t i = 0; i < N; ++i) {
            getline(cin, junk);
        }
        return;
    }
    auto &table = m[tableName];
    
    uint32_t row = table.getNRow();
    table.insert(N);
    cout << "Added " << N << " rows to " << tableName << " from position " << row << " to " << row + N - 1 << '\n';
}

void SQL::print() {
    string tableName, junk;
    cin >> junk >> tableName;
    if (m.find(tableName) == m.end()) {
        cout << "Error during PRINT: " << tableName << " does not name a table in the database\n";
        getline(cin, tableName);
        return;
    }
    auto &table = m[tableName];
    uint32_t N;
    cin >> N;
    vector<uint32_t> colVec;

    for (uint32_t i = 0; i < N; ++i) {
        cin >> junk;
        int index = table.findCol(junk);
        if (index == -1) {
            cout << "Error during PRINT: " << junk << " does not name a column in "<< tableName << '\n';
            getline(cin, tableName);
            return;
        }
        colVec.push_back(static_cast<uint32_t>(index));
    }

    cin >> junk;
    junk == "WHERE" ? table.printWhere(colVec, qMode) : table.printAll(colVec, qMode);   
}

void SQL::deleteWhere() {
    string tableName, junk;
    cin >> junk >> tableName;
    if (m.find(tableName) == m.end()) {
        cout << "Error during DELETE: " << tableName << " does not name a table in the database\n";
        getline(cin, tableName);
        return;
    }

    cin >> junk >> junk;
    auto &table = m[tableName];
    int index = table.findCol(junk);
    if (index == -1) {
        cout << "Error during DELETE: " << junk << " does not name a column in "<< tableName << '\n';
        getline(cin, tableName);
        return;
    }
    table.deleteWhere(index);
}

void SQL::join() {
    string tableName1, tableName2, junk;
    cin >> tableName1 >> junk >> tableName2;
    if (m.find(tableName1) == m.end()) {
        cout << "Error during JOIN: " << tableName1 << " does not name a table in the database\n";
        getline(cin, junk);
        return;
    }

    if (m.find(tableName2) == m.end()) {
        cout << "Error during JOIN: " << tableName2 << " does not name a table in the database\n";
        getline(cin, junk);
        return;
    }
    string colName1, colName2;
    cin >> junk >> colName1;
    auto &table1 = m[tableName1];
    int index1 = table1.findCol(colName1);
    if (index1 == -1) {
        cout << "Error during JOIN: " << colName1 << " does not name a column in "<< tableName1 << '\n';
        getline(cin, junk);
        return;
    }
    cin >> junk >> colName2;
    auto &table2 = m[tableName2];
    int index2 = table2.findCol(colName2);
    if (index2 == -1) {
        cout << "Error during JOIN: " << colName2 << " does not name a column in "<< tableName2 << '\n';
        getline(cin, junk);
        return;
    }
    uint32_t N;
    cin >> junk >> junk >> N;

    vector<printCol> colV;
    colV.reserve(N);
    string colName;
    int fromTable;
    for (uint32_t i = 0; i < N; ++i) {
        cin >> colName >> fromTable;
        if (fromTable == 1) {
            int index = table1.findCol(colName);
            if (index == -1) {
                cout << "Error during JOIN: " << colName << " does not name a column in "<< tableName1 << '\n';
                getline(cin, junk);
                return;
            }
            colV.emplace_back(index, true);

        } else {
            int index = table2.findCol(colName);
            if (index == -1) {
                cout << "Error during JOIN: " << colName << " does not name a column in "<< tableName2 << '\n';
                getline(cin, junk);
                return;
            }
            colV.emplace_back(index, false);
        }
        if (!qMode) {
            cout << colName << ' ';
        }
    }
    if (!qMode) cout << '\n';

    uint32_t numRows = table2.join(table1.getData(), static_cast<uint32_t>(index1), colV, static_cast<uint32_t>(index2), qMode);
    cout << "Printed " << numRows << " rows from joining " << tableName1 << " to " << tableName2 << '\n';
}

void SQL::generate() {
    string colName, junk, indexType, tableName;
    cin >> junk >> tableName;
    if (m.find(tableName) == m.end()) {
        cout << "Error during GENERATE: " << tableName << " does not name a table in the database\n";
        getline(cin, junk);
        return;
    }
    auto &table = m[tableName];
    cin >> indexType >> junk >> junk >> colName;
    int index = table.findCol(colName);
    if (index == -1) {
        cout << "Error during GENERATE: " << colName << " does not name a column in "<< tableName << '\n';
        getline(cin, junk);
        return;
    }
    table.generate(indexType, static_cast<uint32_t>(index));
}

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc,argv);
    cin >> boolalpha;
    cout << boolalpha;
    
    SQL sql;
    sql.getMode(argc, argv);

    string cmd;

    do {
        cout << "% ";
        cin >> cmd;
        switch (cmd[0]) {
        case 'C':
            sql.create();
            break;
        case 'R':
            sql.remove();
            break;
        case 'Q':
            break;
        case 'I':
            sql.insert();
            break;
        case 'P':
            sql.print();
            break;
        case 'D':
            sql.deleteWhere();
            break;
        case 'J':
            sql.join();
            break;
        case 'G':
            sql.generate();
            break;
        case '#':{
            string junk;
            getline(cin, junk);
            break;
        }

        default:
            cout << "Error: unrecognized command\n";
            break;
        }
    } while (cmd != "QUIT");

    cout << "Thanks for being silly!\n";

    return 0;
}
