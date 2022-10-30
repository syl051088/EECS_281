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
                cerr << "Error: unrecognized command" << endl;
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
    size_t N;
    cin >> N;
    vector<EntryType> typeV;
    typeV.reserve(N);
    string type;
    for (size_t i = 0; i < N; ++i) {
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
    for (size_t i = 0; i < N; ++i) {
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
    cin >> junk >> tableName;
    if (m.find(tableName) == m.end()) {
        cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";
        return;
    }
    auto &table = m[tableName];
    size_t N;
    cin >> N >> junk;

    size_t row = table.getNRow();
    table.expend(N);
    row += N;
    
    for (size_t i = 0; i < N; ++i) {
        table.insert();
    }
}

void SQL::print() {

}

void SQL::deleteRow() {

}

void SQL::join() {

}

void SQL::generate() {

}

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    
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
            sql.deleteRow();
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
