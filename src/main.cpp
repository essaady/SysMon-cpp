#include <iostream>
#include <chrono>   // for std::chrono::seconds
#include <thread>   // for std::this_thread::sleep_for

void displayInfo() {
    // Put your code here to display updated information
    std::cout << "Updating info..." << std::endl;
}

int main() {
    while (true) {
        system("clear");
        displayInfo();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
