#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
// Process Structure
// ─────────────────────────────────────────────

struct Process
{
    int id;
    int arrival;
    int burst;
    int priority; // tickets

    int completion;
    int turnaround;
    int response;
    int firstRun;

    Process(int id, int arrival, int burst, int priority)
        : id(id), arrival(arrival), burst(burst), priority(priority),
          completion(0), turnaround(0), response(0), firstRun(-1) {}
};

// ─────────────────────────────────────────────
// Print Results
// ─────────────────────────────────────────────

void printResults(vector<Process> &procs)
{
    cout << "\nProcess  Arr  Burst  Ticket  Completion  Turnaround  Response\n";
    cout << "--------------------------------------------------------------\n";

    double totalTAT = 0, totalRT = 0;

    for (auto &p : procs)
    {
        p.turnaround = p.completion - p.arrival;
        p.response = p.firstRun - p.arrival;

        totalTAT += p.turnaround;
        totalRT += p.response;

        cout << "P" << p.id << "\t"
             << p.arrival << "\t"
             << p.burst << "\t"
             << p.priority << "\t"
             << p.completion << "\t   "
             << p.turnaround << "\t\t"
             << p.response << "\n";
    }

    cout << "\nAverage Turnaround Time: " << totalTAT / procs.size();
    cout << "\nAverage Response Time: " << totalRT / procs.size() << "\n";
}

// ─────────────────────────────────────────────
// Lottery Scheduling
// ─────────────────────────────────────────────

void lottery(vector<Process> procs, int quantum)
{
    srand(42); // fixed seed

    int n = procs.size();
    vector<int> remaining(n);

    for (int i = 0; i < n; i++)
        remaining[i] = procs[i].burst;

    int time = 0;
    int completed = 0;

    while (completed < n)
    {

        // Step 1: eligible processes
        vector<int> eligible;
        for (int i = 0; i < n; i++)
        {
            if (procs[i].arrival <= time && remaining[i] > 0)
                eligible.push_back(i);
        }

        // CPU idle
        if (eligible.empty())
        {
            time++;
            continue;
        }

        // Step 2: total tickets
        int totalTickets = 0;
        for (int idx : eligible)
            totalTickets += procs[idx].priority;

        // Step 3: random ticket
        int winningTicket = rand() % totalTickets;

        // Step 4: find winner
        int cumulative = 0;
        int chosen = -1;

        for (int idx : eligible)
        {
            cumulative += procs[idx].priority;
            if (winningTicket < cumulative)
            {
                chosen = idx;
                break;
            }
        }

        // Step 5: first run
        if (procs[chosen].firstRun == -1)
            procs[chosen].firstRun = time;

        // Step 6: execute
        int run = min(quantum, remaining[chosen]);
        time += run;
        remaining[chosen] -= run;

        // Step 7: completion
        if (remaining[chosen] == 0)
        {
            procs[chosen].completion = time;
            completed++;
        }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────

int main()
{
    vector<Process> processes = {
        {1, 0, 8, 4},
        {2, 1, 4, 2},
        {3, 2, 9, 1},
        {4, 3, 5, 3}};

    int quantum = 3;

    lottery(processes, quantum);

    return 0;
}