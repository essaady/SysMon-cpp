#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <vector>
#include <string>
using namespace std;

class FileUtils {
    public:
        static vector<string> readFileLines(const string& path);
};

#endif