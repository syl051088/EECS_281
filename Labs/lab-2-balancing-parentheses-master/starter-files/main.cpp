/**1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768
 * hello.code
 *
 * Says hello to the world.
 */

#include <deque>
#include <iostream>
using namespace std;
bool checkBalance() {
    deque<char> balanceDec;
    char c;

    while (cin >> c) {
        if (c == '(' || c == '{' || c == '[') {
            balanceDec.push_back(c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (balanceDec.empty()) {
                return false;
            } // if empty
            if (c == ')') {
                if (balanceDec.back() == '(') {
                    balanceDec.pop_back();
                } else {
                    return false;
                } 
            } else if (c == '}') {
                if (balanceDec.back() == '{') {
                    balanceDec.pop_back();
                } else {
                    return false;
                } 
            } else {
                if (balanceDec.back() == '[') {
                    balanceDec.pop_back();
                } else {
                    return false;
                } 
            }
        }
    }
    return balanceDec.empty();
}

int main() {
    if (checkBalance()) {
        cout << "Balanced\n";
    } else {
        cout << "Not balanced\n";
    }

    return 0;
}
