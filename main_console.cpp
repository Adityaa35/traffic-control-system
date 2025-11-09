#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <algorithm>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

string RED = "\033[31m";
string GREEN = "\033[32m";
string YELLOW = "\033[33m";
string CYAN = "\033[36m";
string MAG = "\033[35m";
string RESET = "\033[0m";

void enableVirtualTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
#endif
}

void loader(string msg = "Thinking", int cycles = 6) {
    for (int i=0;i<cycles;i++){
        cout << CYAN << "\r" << msg;
        for (int j=0;j<(i%4);j++) cout<<".";
        cout << RESET << flush;
        // Simple delay
        for (int k=0; k<5000000; k++) {}
    }
    cout << "\r" << string(msg.size()+4,' ') << "\r" << flush;
}

struct Intersection {
    vector<string> dirs = {"North","East","South","West"};
    int cycle_base = 30;
    int min_green = 5;
    int max_green = 60;

    vector<int> allocateGreen(const vector<double>& density) {
        double sum = 0;
        for (double d: density) sum += max(0.0, d);
        vector<int> green(4, min_green);
        if (sum <= 1e-6) return green;
        int total_min = min_green * 4;
        int budget = cycle_base;
        if (budget < total_min) budget = total_min;
        int remaining = budget - total_min;
        for (int i=0;i<4;i++){
            double share = density[i] / sum;
            int add = int(round(share * remaining));
            green[i] += add;
            if (green[i] > max_green) green[i] = max_green;
        }
        int current = accumulate(green.begin(), green.end(), 0);
        while (current < budget) {
            int best=-1; double bestD=-1;
            for (int i=0;i<4;i++) if (green[i] < max_green && density[i] > bestD) { best=i; bestD=density[i]; }
            if (best==-1) break;
            green[best]++; current++;
        }
        while (current > budget) {
            int worst=-1; double worstD=1e18;
            for (int i=0;i<4;i++) if (green[i] > min_green && density[i] < worstD) { worst=i; worstD=density[i]; }
            if (worst==-1) break;
            green[worst]--; current--;
        }
        return green;
    }
};

void printHeader(){
    cout << MAG << "=== Smart Traffic Control System ===" << RESET << "\n\n";
}

int main(){
    enableVirtualTerminal();
    Intersection ctrl;
    printHeader();
    cout << CYAN << "AI Traffic Controller Ready" << RESET << "\n";
    cout << "Commands: auto, manual, exit" << "\n";
    
    while (true) {
        cout << "\nEnter command: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "exit") break;
        
        if (cmd == "auto") {
            cout << GREEN << "Auto mode - generating traffic..." << RESET << "\n";
            loader("Analyzing", 5);
            vector<double> dens(4);
            random_device rd;
            for (int i=0;i<4;i++) dens[i] = 10 + (rd() % 30);
            auto green = ctrl.allocateGreen(dens);
            cout << "\n" << MAG << "=== Traffic Light Allocation ===" << RESET << "\n";
            for (int i=0;i<4;i++){
                cout << ctrl.dirs[i] << ": " << YELLOW << green[i] << "s" << RESET 
                     << " (density: " << dens[i] << " cars/min)\n";
            }
        }
    }
    cout << GREEN << "System shutdown" << RESET << "\n";
    return 0;
}