#include <iostream>
#include <iomanip> 

using namespace std;

void header() {
    const int columnWidth = 20;
    cout << left
         << setw(columnWidth) << "CPU"
         << setw(columnWidth) << "Memory"
         << setw(columnWidth) << "Processes"
         << "\n";
    cout << string(columnWidth * 3, '-') << "\n";
}
void data() {
    const int columnWidth = 20;
    cout << left
         << setw(columnWidth) << "[CPU DATA]"
         << setw(columnWidth) << "[MEMORY DATA]"
         << setw(columnWidth) << "[PROCESS LIST]"
         << "\n";
}

int main() {
    header();
    data();

    return 0;
}