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
    if (m.find(tableName) == m.end()) {
        cout << "Error during CREATE: Cannot create already existing table " << tableName << '\n';
        getline(cin, tableName);
        return;
    }
    int N;
    cin >> N;
    vector<TableEntry> entries;
    string type;
    for (int i = 0; i < N; ++i) {
        cin >> type;
        switch (type[0]) {
        case 's':
            entries.emplace_back(EntryType::String);
            break;
        
        case 'd':
            entries.emplace_back(EntryType::Double);
            break;

        case 'i':
            entries.emplace_back(EntryType::Int);
            break;

        case 'b':
            entries.emplace_back(EntryType::Bool);
            break;
        }
    }

}

void SQL::remove() {

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
            break;

        case 'P':
            break;
        
        case 'D':
            break;
        
        case 'J':
            break;

        case 'G':
            break;
        
        case '#':{
            string junk;
            getline(cin, junk);
            break;
        }

        default:
            cerr << "Invalid command" << endl;
            break;
        }
    } while (cmd != "QUIT");

    cout << "Thanks for being silly!\n";

    return 0;
}
