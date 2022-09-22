#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> preGreatest(vector<int>& vec) {
    deque<int> s;
    s.push_back(vec[0]);
    vec[0] = -1;
    for (size_t i = 1; i < vec.size(); ++i) {
        while (!s.empty() && vec[i] >= s.back()) {
            s.pop_back();
        }
        int temp = vec[i];
        vec[i] = s.empty()? -1 : s.back();
        s.push_back(temp);
    }
    return vec;
}

int main() {
    vector<int> v1 = {18, 19, 12, 14, 13, 12, 15};
    preGreatest(v1);
    for (int i : v1) {
        cout << i << ", ";
        
    }

    return 0;
}