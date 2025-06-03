#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/CpuMonitor.h"

using namespace std;


// The explication in the CpuMonitor.h

std::vector<unsigned long> CpuMonitor::CPU;
std::vector<std::vector<unsigned long>> CpuMonitor::CORES;
unsigned int CpuMonitor::CORES_NUMBER = 0;
unsigned long CpuMonitor::INTERRUPTS = 0;
unsigned long CpuMonitor::CONTEXT_SWITCHES = 0;
unsigned long CpuMonitor::BOOT_TIME = 0;
unsigned long CpuMonitor::PROCESSES = 0;
unsigned long CpuMonitor::PROCESSES_RUNNING = 0;
unsigned long CpuMonitor::PROCESSES_BLOCKED = 0;
std::vector<unsigned long> CpuMonitor::SOFT_IRQS;


bool IsNumber(std::string S){
    for(char c : S){
        if(!(c >= 48 && c <= 57)){
            return false;
        }
    }
    return true;
}

std::vector <unsigned long> CpuMonitor::Split(std::string Line, bool SingleValue){
    return Split(Line, " ", SingleValue);
}

std::vector <unsigned long> CpuMonitor::Split(std::string Line, std::string Delim = " ", bool SingleValue = false){
    std::vector<unsigned long> Values;
    short pos = 0;
    string sWord; 
    while ((pos = Line.find(Delim)) != std::string::npos){
        sWord = Line.substr(0, pos);
        if(IsNumber(sWord) && sWord != ""){
            Values.push_back(stol(sWord));
            if(SingleValue){
                return Values;
            }
        }
        Line.erase(0, pos + Delim.length());   
    }

    if (Line != ""){
        Values.push_back(stol(Line)); 
    }

    return Values;
}

void CpuMonitor::RefreshCpuMonitor(){
    ReadStatFile();
}

void CpuMonitor::ReadStatFile(){
    
    try{
        ifstream file("/proc/stat");

        if(!file.is_open()){
            cerr << "An error occurred while opening /proc/stat" << endl;
            return;
        }
        string line;
        
        getline(file, line);
        CPU = Split(line);

        int i = 0;
        while(getline(file, line)){
            if(line.substr(0, 3) == "cpu"){
                CORES.push_back(Split(line));
                i++;
            }else{
                break;
            }
        }
        CORES_NUMBER = i;
        
        INTERRUPTS = Split(line, true)[0];

        getline(file, line);
        CONTEXT_SWITCHES = Split(line)[0];

        getline(file, line);
        BOOT_TIME = Split(line)[0];
        
        getline(file, line);
        PROCESSES = Split(line)[0];
        
        getline(file, line);
        PROCESSES_RUNNING = Split(line)[0];
        
        getline(file, line);
        PROCESSES_BLOCKED = Split(line)[0];
        
        getline(file, line);
        SOFT_IRQS = Split(line);
    }catch(const exception& e){
        cerr << "Error : " << e.what() << endl;
    }
    

}

