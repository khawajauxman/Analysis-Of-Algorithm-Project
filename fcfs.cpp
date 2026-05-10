#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int response;
};

void fcfs(vector<Process>& p) {
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arrival < b.arrival;
    });

    int time = 0;
    for (int i = 0; i < p.size(); i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        p[i].response   = time - p[i].arrival;
        p[i].completion = time + p[i].burst;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting    = p[i].turnaround - p[i].burst;
        time = p[i].completion;
    }
}

void showResults(vector<Process> p) {
    cout << "\n";
    cout << left;
    cout << setw(6)  << "PID"
         << setw(10) << "Arrival"
         << setw(8)  << "Burst"
         << setw(13) << "Completion"
         << setw(13) << "Turnaround"
         << setw(10) << "Waiting"
         << setw(10) << "Response" << "\n";
    cout << string(70, '-') << "\n";

    double tat = 0, wt = 0;
    for (int i = 0; i < p.size(); i++) {
        cout << setw(6)  << p[i].pid
             << setw(10) << p[i].arrival
             << setw(8)  << p[i].burst
             << setw(13) << p[i].completion
             << setw(13) << p[i].turnaround
             << setw(10) << p[i].waiting
             << setw(10) << p[i].response << "\n";
        tat += p[i].turnaround;
        wt  += p[i].waiting;
    }

    cout << string(70, '-') << "\n";
    cout << fixed << setprecision(2);
    cout << "Avg Turnaround Time: " << tat / p.size() << " ms\n";
    cout << "Avg Waiting Time   : " << wt  / p.size() << " ms\n";
}

void showGantt(vector<Process> p) {
    cout << "\nGantt Chart:\n|";
    for (int i = 0; i < p.size(); i++)
        cout << " P" << p[i].pid << " |";
    cout << "\n0";
    for (int i = 0; i < p.size(); i++)
        cout << "    " << p[i].completion;
    cout << "\n";
}

void runEmpirical() {
    int sizes[] = {10, 50, 100, 500, 1000, 5000, 10000};
    cout << "\nEmpirical Analysis:\n";
    cout << left << setw(12) << "Input Size" << setw(20) << "Time (microseconds)" << "\n";
    cout << string(32, '-') << "\n";

    for (int n : sizes) {
        vector<Process> procs(n);
        for (int i = 0; i < n; i++) {
            procs[i].pid     = i + 1;
            procs[i].arrival = i * 2;
            procs[i].burst   = (i % 10) + 1;
        }

        auto start = chrono::high_resolution_clock::now();
        fcfs(procs);
        auto end = chrono::high_resolution_clock::now();
        long long us = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << setw(12) << n << setw(20) << us << "\n";
    }
}

int main() {
    cout << "===== Test Case 1 =====\n";
    vector<Process> tc1 = {{1,0,5},{2,1,3},{3,2,8},{4,3,6}};
    fcfs(tc1);
    showResults(tc1);
    showGantt(tc1);

    cout << "\n===== Test Case 2 =====\n";
    vector<Process> tc2 = {{1,0,4},{2,0,3},{3,0,5},{4,0,2}};
    fcfs(tc2);
    showResults(tc2);
    showGantt(tc2);

    cout << "\n===== Test Case 3 =====\n";
    vector<Process> tc3 = {{1,0,2},{2,3,4},{3,8,1},{4,10,6},{5,12,3}};
    fcfs(tc3);
    showResults(tc3);
    showGantt(tc3);

    runEmpirical();

    return 0;
}
