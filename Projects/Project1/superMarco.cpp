#include <iostream>
#include <deque>
#include <climits>
#include <string>
#include <getopt.h>
#include "xcode_redirect.hpp"
using namespace std;

struct Tile{
public:
    bool isVisited = 0;
    char value;
};

class solver{
public:
    void solve(string outputMode) {
    deque<Tile> tileDeque;
    while (!tileDeque.empty()) {
        // Tile current;
    }
    if (outputMode == "q") {
        // tileDeque.push_back(mapVec);
    } else {

    }
}

private:
    // vector<vector<vector<Tile>>> mapVec;
    char mode;
    unsigned char R;
    uint32_t N;
};



// mapVec.reserve(R * N * N);


void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
} // printHelp()


string getMode(int argc, char * argv[]) {
    bool modeSpecified = false;
    string mode;

    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "queue", required_argument, nullptr, 'q' },
        { "stack", no_argument, nullptr, 's' },
        { nullptr, 0,                 nullptr, '\0' }
    };

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "q:s:h", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            return 0;
        case 'q':
            mode = optarg;
            if (mode != "stack") {

            }
        case 's':
            mode = optarg;
            if (mode != "resize" && mode != "reserve" && mode != "nosize") {
                // The first line of error output has to be a 'fixed' message for the autograder
                // to show it to you.
                cerr << "Error: invalid mode" << "\n";
                // The second line can provide more information, but you won't see it on the AG.
                cerr << "  I don't know recognize: " << mode << "\n";
                exit(1);
            } // if
            modeSpecified = true;
            break;

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch
    } // while

    if (!modeSpecified) {
        cerr << "Error: no mode specified" << endl;
        exit(1);
    } // if

    return mode;
} // getMode()

void readFile() {
    cin >> mode;
    cin >> R;
    cin >> N;

    // if (mode == 'M') {
    //     readM();
    // } else {
    //     readL();
    // }
}

void readM() {
    string s;
    uint32_t length = 0;
    while (getline(cin, s)) {
        if (s[0] == '/') {
            continue;
        }

        if (length >= N - 1) {
            length = 0;
        } else {
            ++length;
        }
    }
}

void readL() {
    string s;
    while (getline(cin, s)) {
        if (s[0] == '/') {
            continue;
        }


    }
}






int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    // string oMode;
    readFile();

    // oMode = getMode(argc, argv);

    // if (oMode == "stack" || oMode == "queue")
    //     solve(oMode);
    // // else if (oMode == "reserve")
    // //     readWithReserve(data);
    // // else if (oMode == "nosize")
    // //     readWithNoSize(data);
    // else {
    //     cerr << "Invalid mode \"" << mode << "\" specified, should not be possible to reach this point!" << endl;
    //     return 1;
    // } // if

    cout << "Mode is: " << mode << "\n";
    cout << "Number of rooms: " << R << "\n";
    cout << "Size of room: " << N << "\n";
    return 0;
}