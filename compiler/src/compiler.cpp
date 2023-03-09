#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream file("res/Test1.cp");
    char c;

    if (!file.is_open()) {
        cout << "Failed to open file" << endl;
        return 1;
    }

    while (file.get(c)) {
        cout << c;
    }

    file.close();

    return 0;
}
