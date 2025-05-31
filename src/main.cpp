#include "../include/FileUtils.h"
#include <iostream>

using namespace std;

int main() {
    vector<string> lignes = FileUtils::readFileLines("/proc/cpuinfo");

    for (const string& ligne : lignes) {
        cout << ligne << endl;
    }

    return 0;
}