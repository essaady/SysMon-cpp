#include <iostream>
#include <queue>
#include <stack>

struct Process {
    int id;
    std::string name;
    Process(int id, std::string name) : id(id), name(name) {}
};

// FIFO 
void fifoScheduling(std::queue<Process> q) {
    std::cout << "FIFO Scheduling:\n";
    while (!q.empty()) {
        Process p = q.front();
        q.pop();
        std::cout << "Processing: " << p.id << " - " << p.name << "\n";
    }
}

// LIFO 
void lifoScheduling(std::stack<Process> s) {
    std::cout << "\nLIFO Scheduling:\n";
    while (!s.empty()) {
        Process p = s.top();
        s.pop();
        std::cout << "Processing: " << p.id << " - " << p.name << "\n";
    }
}

int main() {
    // Example processes
    std::queue<Process> fifoQueue;
    std::stack<Process> lifoStack;

    // Adding processes
    fifoQueue.push(Process(1, "Process A"));
    fifoQueue.push(Process(2, "Process B"));
    fifoQueue.push(Process(3, "Process C"));

    lifoStack.push(Process(1, "Process A"));
    lifoStack.push(Process(2, "Process B"));
    lifoStack.push(Process(3, "Process C"));

    fifoScheduling(fifoQueue);
    lifoScheduling(lifoStack);

    return 0;
}