#include <iostream>
#include <fstream>
#include <vector>
#include <string>
class FileUtils {
public:
    static std::vector<std::string> readFileLines(const std::string& path) {
        std::vector<std::string> lines;
        std::ifstream file(path);

        if (!file) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier " << path << std::endl;
            return lines;
}
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
}
        file.close();
        return lines;
}
};
int main() {
    std::vector<std::string> lines = FileUtils::readFileLines("/proc/cpuinfo");

    if (lines.empty()) {
        std::cerr << "Aucune donnée disponible ou erreur lors de la lecture du fichier." << std::endl;
        return 1;
}
    for (const auto& line: lines) {
        std::cout << line << std::endl;
}
    return 0;
}
