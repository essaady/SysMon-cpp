#include "../include/ProcessMonitor.h"
#include "../include/SysMon.h"
#include <string>
#include <vector>


using namespace std;

// Constructeur
ProcessMonitor::ProcessMonitor() {
    // Initialisation des ressources si nécessaire
}


// Destructeur
ProcessMonitor::~ProcessMonitor() {
    // Libération des ressources si nécessaire

}


std::string ProcessMonitor::getProcessRaw()
{
    std::cout << "\x1b[42m Name       " << "PID        \x1b[0m\n";
    std::string path = "/proc";
    int processCount = 0;
    for (auto const &entry : std::filesystem::directory_iterator(path))
    {
        std::string newPath = entry.path();
        std::string pathFileName = entry.path().filename();
        if (isNumber(pathFileName))
        {
            std::string fileContent = SysMon::getInfo(newPath + "/status");
            if(fileContent == " "){
                continue;
            }
            std::istringstream iss(fileContent);

            std::vector<std::string> info = getVector(iss);
            processCount++;

            std::cout << info[1] << "    " << info[12] << std::endl;
        }
    }


    std::cout << "There are : " << processCount << " processes\n";
    return " ";
}

bool ProcessMonitor::update(){
    //To-Do
    getProcessRaw();
    apList.clear();  
    nbrProcess = 0;
    return true;
}

AP ProcessMonitor::getProcess(int index) const {
    if (index >= 0 && index < static_cast<int>(apList.size())) {
        return apList[index];
    }
    throw std::out_of_range("Index hors limites !");
}

std::string ProcessMonitor::getProcessInfo() const {
    std::ostringstream oss;
    oss << "\x1b[42m Name       PID        \x1b[0m\n";
    for (const auto& proc : apList) {
        oss << proc.name << "    " << proc.pid << "\n";
    }
    oss << "There are : " << nbrProcess << " processes\n";
    return oss.str();
}
