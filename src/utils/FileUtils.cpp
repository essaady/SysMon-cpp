#include "../../include/FileUtils.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

vector<string> FileUtils::readFileLines(const string& path) {
    vector<string> lines;
    ifstream file(path);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        cout << " impossible ouvrir cet fichier  " << path << endl;
    }

    return lines;
}
