/*
 * OS CPU Scheduling Simulator
 * Algorithms: FIFO, SJF, STCF (Preemptive SJF), Round Robin
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <numeric>
using namespace std;

// ─────────────────────────────────────────────
//  Data structures
// ─────────────────────────────────────────────

struct Process {
    int id;
    int arrival;
    int burst;

    // filled in by scheduler
    int completion;
    int turnaround;
    int response;
    int firstRun;   // tracks when process first got CPU

    Process(int id, int arrival, int burst)
        : id(id), arrival(arrival), burst(burst),
          completion(0), turnaround(0), response(0), firstRun(-1) {}
};



// ─────────────────────────────────────────────
//  Display helpers
// ─────────────────────────────────────────────



void printResults(vector<Process>& procs) {
    cout << "\n  " << left
         << setw(10) << "Process"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(14) << "Completion"
         << setw(14) << "Turnaround"
         << setw(12) << "Response"
         << "\n";
    cout << "  " << string(70, '-') << "\n";

    double totalTAT = 0, totalRT = 0;
    for (auto& p : procs) {
        p.turnaround = p.completion - p.arrival;
        p.response   = p.firstRun   - p.arrival;
        totalTAT += p.turnaround;
        totalRT  += p.response;

        cout << "  " << left
             << setw(10) << ("P" + to_string(p.id))
             << setw(10) << p.arrival
             << setw(10) << p.burst
             << setw(14) << p.completion
             << setw(14) << p.turnaround
             << setw(12) << p.response
             << "\n";
    }

    int n = procs.size();
    cout << "\n  Avg Turnaround Time : " << fixed << setprecision(2) << totalTAT / n;
    cout << "\n  Avg Response Time   : " << fixed << setprecision(2) << totalRT  / n;
    cout << "\n\n";
}

// ─────────────────────────────────────────────
//  1. FIFO — First In First Out
// ─────────────────────────────────────────────

void fifo(vector<Process> procs) {
    cout << "\n============================\n";
    cout << "  FIFO Scheduling\n";
    cout << "============================";

    // sort by arrival time
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    int time = 0;

    for (auto& p : procs) {
        if (time < p.arrival)
            time = p.arrival;

        if (p.firstRun == -1) p.firstRun = time;
        time += p.burst;
        p.completion = time;
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
//  2. SJF — Shortest Job First (non-preemptive)
// ─────────────────────────────────────────────

void sjf(vector<Process> procs) {
    cout << "\n============================\n";
    cout << "  SJF Scheduling (Non-Preemptive)\n";
    cout << "============================";

    int n = procs.size();
    vector<bool> done(n, false);
    int time = 0, completed = 0;

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && procs[i].arrival <= time) {
                if (idx == -1 || procs[i].burst < procs[idx].burst)
                    idx = i;
            }
        }

        if (idx == -1) { time++; continue; }

        Process& p = procs[idx];
        if (p.firstRun == -1) p.firstRun = time;
        time += p.burst;
        p.completion = time;
        done[idx] = true;
        completed++;
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
//  3. STCF — Shortest Time to Completion First
//     (Preemptive SJF)
// ─────────────────────────────────────────────

void stcf(vector<Process> procs) {
    cout << "\n============================\n";
    cout << "  STCF Scheduling (Preemptive SJF)\n";
    cout << "============================";

    int n = procs.size();
    vector<int> remaining(n);
    for (int i = 0; i < n; i++) remaining[i] = procs[i].burst;

    int time = 0, completed = 0;

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && procs[i].arrival <= time) {
                if (idx == -1 || remaining[i] < remaining[idx])
                    idx = i;
            }
        }

        if (idx == -1) { time++; continue; }

        if (procs[idx].firstRun == -1) procs[idx].firstRun = time;
        remaining[idx]--;
        time++;

        if (remaining[idx] == 0) {
            procs[idx].completion = time;
            completed++;
        }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
//  4. Round Robin
// ─────────────────────────────────────────────

void roundRobin(vector<Process> procs, int quantum) {
    cout << "\n============================\n";
    cout << "  Round Robin Scheduling (Quantum = " << quantum << ")\n";
    cout << "============================";

    int n = procs.size();
    vector<int> remaining(n);

    // sort by arrival time so all linear scans enqueue in arrival order
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    for (int i = 0; i < n; i++) remaining[i] = procs[i].burst;

    queue<int> rq;
    vector<bool> inQueue(n, false);
    int time = 0, completed = 0;

    while (completed < n) {

        // enqueue all processes that have arrived by this time, in arrival order
        for (int j = 0; j < n; j++)
            if (!inQueue[j] && remaining[j] > 0 && procs[j].arrival <= time)
                { rq.push(j); inQueue[j] = true; }

        // no process ready yet, advance one tick
        if (rq.empty()) { time++; continue; }

        // run the front process for one quantum
        int i = rq.front(); rq.pop(); inQueue[i] = false;
        if (procs[i].firstRun == -1) procs[i].firstRun = time;

        int run = min(quantum, remaining[i]);
        int prevTime = time;
        time += run;
        remaining[i] -= run;

        // enqueue any process that arrived during (prevTime, time]
        for (int j = 0; j < n; j++)
            if (!inQueue[j] && remaining[j] > 0 && procs[j].arrival > prevTime && procs[j].arrival <= time)
                { rq.push(j); inQueue[j] = true; }

        if (remaining[i] == 0) { procs[i].completion = time; completed++; }
        else { rq.push(i); inQueue[i] = true; }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────

int main() {
    // Define processes: {id, arrival, burst}
    vector<Process> processes = {
        {1, 0, 8},
        {2, 1, 4},
        {3, 2, 9},
        {4, 3, 5},
    };

    cout << "  Processes: ";
    for (auto& p : processes)
        cout << "[P" << p.id << " arr=" << p.arrival << " burst=" << p.burst << "] ";
    cout << "\n";

    int quantum = 3;   // time slice for Round Robin

    fifo(processes);
    sjf(processes);
    stcf(processes);
    roundRobin(processes, quantum);

    return 0;
}