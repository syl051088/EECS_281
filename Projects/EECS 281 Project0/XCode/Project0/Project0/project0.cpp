// 39E995D00DDE0519FDF5506EED902869AEC1C39E

// EECS 281, Project 0
// Learn about:
//   1) Command-line processing with getopt_long()
//   2) The vector member functions .resize() and .reserve()
//   3) The proper way to read data until end-of-file


#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#include "xcode_redirect.hpp"

// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
} // printHelp()


// Finish this function, look for the individual TODO comments internally.
// Process the command line; the only thing we need to return is the mode
// when the user specifies the -m/--mode option.
string getMode(int argc, char * argv[]) {
    bool modeSpecified = false;
    string mode;

    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        // Fill in two lines, for the "mode" ('m') and
        // the "help" ('h') options.
        // ./project0 -m nosize
        // ./project0 --help
        { "mode", required_argument, nullptr, 'm' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr, 0,                 nullptr, '\0' }
    };

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);

        case 'm':
            mode = optarg;
            if (mode != "resize" && mode != "reserve" && mode != "nosize") {
                // The first line of error output has to be a 'fixed' message for the autograder
                // to show it to you.
                cerr << "Error: invalid mode" << endl;
                // The second line can provide more information, but you won't see it on the AG.
                cerr << "  I don't know recognize: " << mode << endl;
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


// Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.resize() to set the current size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using a subscript
void readWithResize(vector<double> &data) {
    // step 1
    size_t size;
    cin >> size;

    // step 2
    data.resize(size);

    // step 3
    for (size_t i = 0; i < size; ++i) {
        cin >> data[i];
    } // for i

} // readWithResize()


// Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.reserve() to set the maximum size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using data.push_back()
void readWithReserve(vector<double> &data) {
    size_t size;
    cin >> size;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        double temp;
        cin >> temp;
        data.push_back(temp);
    } // for i

} // readWithReserve()


// TODO: Write this function.  It should do the following:
//   Use a while loop to read in the values, storing them
//   into the vector using data.push_back()
void readWithNoSize(vector<double> &data) {
    double temp;
    while (cin >> temp) {
        data.push_back(temp);
    } // while

} // readWithNoSize()


// This function is already done.
double getAverage(const vector<double> &data) {
    double average, sum = 0;

    for (size_t i = 0; i < data.size(); ++i)
        sum += data[i];

    average = sum / static_cast<double>(data.size());
    return average;
} // getAverage()


// This function is already done.  Note that we have to sort the vector
// before we can find the median.
double getMedian(vector<double> &data) {
    // Sort the data
    sort(data.begin(), data.end());

    // Figure out if vector size is even or odd
    if (data.size() % 2 == 1)
        return data[data.size() / 2];

    // Even size, average 2 middle values
    auto mid = data.size() / 2;
    double v1 = data[mid];
    double v2 = data[mid - 1];
    return (v1 + v2) / 2;
} // getMedian()


// This function is already done.
int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    string mode;
    vector<double> data;

    // Set two digits of precision on output
    cout << fixed << showpoint << setprecision(2);

    mode = getMode(argc, argv);

    if (mode == "resize")
        readWithResize(data);
    else if (mode == "reserve")
        readWithReserve(data);
    else if (mode == "nosize")
        readWithNoSize(data);
    else {
        cerr << "Invalid mode \"" << mode << "\" specified, should not be possible to reach this point!" << endl;
        return 1;
    } // if

    if (data.size() == 0)
        cout << "No data => no statistics!" << '\n';
    else {
        cout << "Average: " << getAverage(data) << '\n';
        cout << " Median: " << getMedian(data)  << '\n';
    } // else

    return 0;
} // main()
