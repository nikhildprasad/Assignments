#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
};

// Function for First-Come-First-Serve (FCFS) Scheduling
void fcfs(vector<Process> &processes) {
    int current_time = 0;
    for (auto &p : processes) {
        if (current_time < p.arrival_time)
            current_time = p.arrival_time;
        p.waiting_time = current_time - p.arrival_time;
        p.completion_time = current_time + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        current_time += p.burst_time;
    }
}

// Function for Shortest Job First (SJF) Scheduling
void sjf(vector<Process> &processes) {
    vector<Process> ready_queue;
    int current_time = 0;
    while (!processes.empty() || !ready_queue.empty()) {
        for (auto it = processes.begin(); it != processes.end(); ) {
            if (it->arrival_time <= current_time) {
                ready_queue.push_back(*it);
                it = processes.erase(it);
            } else {
                ++it;
            }
        }
        if (!ready_queue.empty()) {
            auto shortest = min_element(ready_queue.begin(), ready_queue.end(), [](const Process &a, const Process &b) {
                return a.burst_time < b.burst_time;
            });
            Process p = *shortest;
            ready_queue.erase(shortest);
            p.waiting_time = current_time - p.arrival_time;
            p.completion_time = current_time + p.burst_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            current_time += p.burst_time;
            cout << "Process " << p.id << ": Waiting Time = " << p.waiting_time 
                 << ", Turnaround Time = " << p.turnaround_time << endl;
        } else {
            current_time++;
        }
    }
}

// Function for Round Robin Scheduling
void round_robin(vector<Process> &processes, int time_quantum) {
    queue<Process> ready_queue;
    int current_time = 0;
    for (auto &p : processes)
        ready_queue.push(p);

    while (!ready_queue.empty()) {
        Process p = ready_queue.front();
        ready_queue.pop();
        if (current_time < p.arrival_time)
            current_time = p.arrival_time;
        int time_used = min(time_quantum, p.burst_time);
        p.burst_time -= time_used;
        current_time += time_used;
        if (p.burst_time > 0) {
            ready_queue.push(p);
        } else {
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - (p.burst_time + time_used);
            cout << "Process " << p.id << ": Waiting Time = " << p.waiting_time 
                 << ", Turnaround Time = " << p.turnaround_time << endl;
        }
    }
}

// Main function
int main() {
    int n, choice, time_quantum;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter arrival time and burst time for Process " << i + 1 << ": ";
        cin >> processes[i].arrival_time >> processes[i].burst_time;
        processes[i].id = i + 1;
    }

    cout << "Choose Scheduling Algorithm:\n";
    cout << "1. First-Come-First-Serve (FCFS)\n";
    cout << "2. Shortest Job First (SJF)\n";
    cout << "3. Round Robin (RR)\n";
    cin >> choice;

    if (choice == 3) {
        cout << "Enter time quantum for Round Robin: ";
        cin >> time_quantum;
    }

    // Sort processes by arrival time for FCFS and SJF
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    switch (choice) {
        case 1:
            fcfs(processes);
            break;
        case 2:
            sjf(processes);
            break;
        case 3:
            round_robin(processes, time_quantum);
            break;
        default:
            cout << "Invalid choice!" << endl;
    }

    cout << "\nFinal Results:\n";
    for (const auto &p : processes) {
        cout << "Process " << p.id << ": Waiting Time = " << p.waiting_time 
             << ", Turnaround Time = " << p.turnaround_time << endl;
    }

    return 0;
}
