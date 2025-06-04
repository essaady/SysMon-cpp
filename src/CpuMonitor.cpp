#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/CpuMonitor.h"


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
    std::string sWord; 
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

CpuMonitor::CpuMonitor(){
    ReadStatFile();
    CPU.usagePerCpu = new float[CPU.nbrCPU];
}

CpuMonitor::~CpuMonitor(){
    delete[] CPU.usagePerCpu;
}


bool CpuMonitor::update(){
    PREV_CPU = CURR_CPU;
    PREV_CORES = CURR_CORES;
    
    if(!ReadStatFile()){
        return 0;
    }

    if(!ReadFreqFiles()){
        return 0;
    }

    ImplementCpuValues();
    return 1;
}

bool CpuMonitor::ReadStatFile(){
    
    try{
        std::ifstream file("/proc/stat");

        if(!file.is_open()){
            std::cerr << "An error occurred while opening /proc/stat" << std::endl;
            return 0;
        }
        std::string line;
        
        getline(file, line);
        CURR_CPU = Split(line);

        CURR_CORES.clear();
        int i = 0;
        while(getline(file, line)){
            if(line.substr(0, 3) == "cpu"){
                CURR_CORES.push_back(Split(line));
                i++;
            }else{
                break;
            }
        }
        CPU.nbrCPU = i;
        
        
    }catch(const std::exception& e){
        std::cerr << "Error : " << e.what() << std::endl;
    }
    
    return 1;

}



bool CpuMonitor::ReadFreqFiles(){
    std::string CurrFreqFileName;
    std::string MaxFreqFileName;
    std::string line;

    CURR_CORES_FREQ.clear();
    MAX_CORES_FREQ.clear();
    for(int i=0; i<CPU.nbrCPU; i++){
        CurrFreqFileName = "/sys/devices/system/cpu/cpu" + std::to_string(i) + "/cpufreq/scaling_cur_freq";
        MaxFreqFileName = "/sys/devices/system/cpu/cpu" + std::to_string(i) + "/cpufreq/cpuinfo_max_freq";

        std::ifstream CurrFreqfile(CurrFreqFileName);

        if(!CurrFreqfile.is_open()){
            std::cerr << "An error occurred while opening " << CurrFreqFileName << std::endl;
            return 0;
        }

        getline(CurrFreqfile, line);

        if(IsNumber(line) && line != ""){
            CURR_CORES_FREQ.push_back(stod(line) / 1000000);
        }


        std::ifstream MaxFreqfile(MaxFreqFileName);

        if(!MaxFreqfile.is_open()){
            std::cerr << "An error occurred while opening " << MaxFreqFileName << std::endl;
            return 0;
        }

        getline(MaxFreqfile, line);

        if(IsNumber(line) && line != ""){
            MAX_CORES_FREQ.push_back(stod(line) / 1000000);
        }
    
    }
    
    return 1;
}



void CpuMonitor::ImplementCpuValues(){

    float Sum = 0;

    for(float v : CURR_CORES_FREQ){
        Sum += v;
    }

    CPU.frequency = float(Sum) / CPU.nbrCPU; 

    Sum = 0;
    for(float v : MAX_CORES_FREQ){
        Sum += v;
    }

    CPU.frequencyMax = float(Sum) / CPU.nbrCPU; 
    

    unsigned long long prev_idle;
    unsigned long long curr_idle;

    unsigned long long prev_used;
    unsigned long long curr_used;

    unsigned long long prev_total;
    unsigned long long curr_total;

    unsigned long long idle_delta;
    unsigned long long total_delta;

    // For the entire CPU 

    prev_idle = PREV_CPU[States::idle] + PREV_CPU[States::iowait];
    
    curr_idle = CURR_CPU[States::idle] + CURR_CPU[States::iowait];

    prev_used = PREV_CPU[States::user] + PREV_CPU[States::nice] + 
                PREV_CPU[States::system] + PREV_CPU[States::irq] +
                PREV_CPU[States::softirq] + PREV_CPU[States::steal];   
 
    curr_used = CURR_CPU[States::user] + CURR_CPU[States::nice] + 
                CURR_CPU[States::system] + CURR_CPU[States::irq] +
                CURR_CPU[States::softirq] + CURR_CPU[States::steal];


    prev_total = prev_idle + prev_used;

    curr_total = curr_idle + curr_used;

    total_delta = curr_total - prev_total;

    idle_delta = curr_idle - prev_idle;



    CPU.usageCPU = (float(total_delta - idle_delta) / float(total_delta) * 100.0);


    // for the cores

    for(int i=0; i<CPU.nbrCPU; i++){


        prev_idle = PREV_CORES[i][States::idle] + PREV_CORES[i][States::iowait];
        curr_idle = CURR_CORES[i][States::idle] + CURR_CORES[i][States::iowait];

        prev_used = PREV_CORES[i][States::user] + PREV_CORES[i][States::nice] + 
                    PREV_CORES[i][States::system] + PREV_CORES[i][States::irq] +
                    PREV_CORES[i][States::softirq] + PREV_CORES[i][States::steal];   

        curr_used = CURR_CORES[i][States::user] + CURR_CORES[i][States::nice] + 
                    CURR_CORES[i][States::system] + CURR_CORES[i][States::irq] +
                    CURR_CORES[i][States::softirq] + CURR_CORES[i][States::steal];


        prev_total = prev_idle + prev_used;

        curr_total = curr_idle + curr_used;

        total_delta = curr_total - prev_total;

        idle_delta = curr_idle - prev_idle;

        CPU.usagePerCpu[i] = (float(total_delta - idle_delta) / total_delta) * 100.0;

    }

    rawCPU = "";
    
    rawCPU += "frequency "  + std::to_string(CPU.frequency) + ",";
    rawCPU += "frequency_max "  + std::to_string(CPU.frequencyMax) + ",";
    rawCPU += "usage_cpu " + std::to_string(CPU.usageCPU) + ",";
    for(int i=0; i<CPU.nbrCPU; i++){
        rawCPU += "usage_cpu" + std::to_string(i) + " " + std::to_string(CPU.usageCPU) + ",";
    }
    rawCPU += "nbrCPU " + std::to_string(CPU.nbrCPU);


}

float CpuMonitor::getCpuUsage(){
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq(){
    return CPU.frequency;
}

std::string CpuMonitor::getCpuInfo(){
    return rawCPU;
}

