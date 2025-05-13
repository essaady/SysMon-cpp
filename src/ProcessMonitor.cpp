#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <map>

struct ProcessInfo {
    int pid;
    std::string command;
    unsigned long prev_time;
    unsigned long curr_time;
    double cpu_percent;
};

unsigned long get_total_cpu_time() {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    std::istringstream iss(line);
    std::string cpu;
    unsigned long total = 0, val;
    iss >> cpu;
    while (iss >> val) {
        total += val;
    }
    return total;
}

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::map<int, ProcessInfo> get_process_times() {
    std::map<int, ProcessInfo> processes;
    DIR* proc = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(proc)) != nullptr) {
        if (entry->d_type == DT_DIR && is_number(entry->d_name)) {
            int pid = std::stoi(entry->d_name);
            std::ifstream stat_file("/proc/" + std::to_string(pid) + "/stat");
            if (!stat_file) continue;

            std::string line;
            std::getline(stat_file, line);
            std::istringstream iss(line);
            std::string comm;
            std::string tmp;
            unsigned long utime, stime;
            iss >> tmp >> comm;
            // Skip until fields 14 and 15 (utime and stime)
            for (int i = 0; i < 11; ++i) iss >> tmp;
            iss >> utime >> stime;

            ProcessInfo info;
            info.pid = pid;
            info.command = comm.substr(1, comm.size() - 2); // Remove parentheses
            info.prev_time = utime + stime;
            processes[pid] = info;
        }
    }

    closedir(proc);
    return processes;
}

int main() {
    auto procs1 = get_process_times();
    unsigned long total_cpu1 = get_total_cpu_time();

    sleep(1); // wait 1 second

    auto procs2 = get_process_times();
    unsigned long total_cpu2 = get_total_cpu_time();

    unsigned long total_diff = total_cpu2 - total_cpu1;
    std::vector<ProcessInfo> final_list;

    for (auto& [pid, proc] : procs2) {
        if (procs1.find(pid) != procs1.end()) {
            proc.curr_time = proc.prev_time;
            proc.prev_time = procs1[pid].prev_time;
            unsigned long delta = proc.curr_time - proc.prev_time;
            proc.cpu_percent = (double(delta) / total_diff) * 100.0;
            final_list.push_back(proc);
        }
    }

    std::sort(final_list.begin(), final_list.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpu_percent > b.cpu_percent;
    });

    std::cout << "PID\tCOMMAND\t\tCPU %\n";
    for (int i = 0; i < 5 && i < final_list.size(); ++i) {
        std::cout << final_list[i].pid << "\t"
                  << final_list[i].command << "\t\t"
                  << std::fixed << std::setprecision(2)
                  << final_list[i].cpu_percent << "\n";
    }

    return 0;
}
