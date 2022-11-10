// Testing file provided for EECS 281 Lab 7

#include "hashtable.h"
#include <string>
#include <iostream>

int main() {

    // VERY BASIC EXAMPLE (will likely not compile with default/empty hashtable.h file)
    HashTable<std::string, int> midterm;
    std::cout << midterm.insert("sam", 50) << " ";
    std::cout << midterm.insert("fee", 100) << " ";
    std::cout << midterm.insert("milo", 95) << " ";
        std::cout << midterm.insert("s1m", 50) << " ";
    std::cout << midterm.insert("fe1e", 100) << " ";
    std::cout << midterm.insert("mi1lo", 95) << " ";
        std::cout << midterm.insert("sa2m", 50) << " ";
    std::cout << midterm.insert("f2ee", 100) << " ";
    std::cout << midterm.insert("mi2lo", 95) << " ";
        std::cout << midterm.insert("sa3m", 50) << " ";
    std::cout << midterm.insert("f3ee", 100) << " ";
    std::cout << midterm.insert("mi3lo", 95) << " ";
        std::cout << midterm.insert("s4am", 50) << " ";
    std::cout << midterm.insert("f4ee", 100) << " ";
    std::cout << midterm.insert("mi4lo", 95) << " ";
        std::cout << midterm.insert("sa5m", 50) << " ";
    std::cout << midterm.insert("f5ee", 100) << " ";
    std::cout << midterm.insert("mi5lo", 95) << " ";
    


    std::cout << midterm.insert("sa21m", 50) << " ";
    std::cout << midterm.insert("fe21e", 100) << " ";
    std::cout << midterm.insert("mi21lo", 95) << " ";
        std::cout << midterm.insert("s113m", 50) << " ";
    std::cout << midterm.insert("fe131e", 100) << " ";
    std::cout << midterm.insert("mi113lo", 95) << " ";
        std::cout << midterm.insert("sa241m", 50) << " ";
    std::cout << midterm.insert("f241ee", 100) << " ";
    std::cout << midterm.insert("mi421lo", 95) << " ";
        std::cout << midterm.insert("sa153m", 50) << " ";
    std::cout << midterm.insert("f315ee", 100) << " ";
    std::cout << midterm.insert("mi153lo", 95) << " ";
        std::cout << midterm.insert("s461am", 50) << " ";
    std::cout << midterm.insert("f41e6e", 100) << " ";
    std::cout << midterm.insert("mi41l6o", 95) << " ";
        std::cout << midterm.insert("sa751m", 50) << " ";
    std::cout << midterm.insert("f5e71e", 100) << " ";
    std::cout << midterm.insert("mi571lo", 95) << " ";
    std::cout << midterm.insert("gab1e", 88) << " \n";
    std::cout << midterm["sam"] << " ";
    std::cout << midterm["fee"] << " ";
    std::cout << midterm["milo"] << " ";
    std::cout << midterm["gabe"] << " \n";
    std::cout << midterm.erase("sam") << " ";
    std::cout << midterm["sam"] << "\n";
    // ADD MORE TESTS OF YOUR OWN

    return 0;
}
