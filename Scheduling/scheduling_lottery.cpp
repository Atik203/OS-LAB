/*
 * OS CPU Scheduling Simulator
 * Algorithms: FIFO, SJF, STCF, Round Robin, Lottery
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstdlib>

using namespace std;

// ─────────────────────────────────────────────
// Process Structure
// ─────────────────────────────────────────────

struct Process
{
    int id;
    int arrival;
    int burst;
    int tickets;

    int completion;
    int turnaround;
    int response;
    int firstRun;

    Process(int id, int arrival, int burst, int tickets = 1)
        : id(id), arrival(arrival), burst(burst), tickets(tickets),
          completion(0), turnaround(0), response(0), firstRun(-1) {}
};

// ─────────────────────────────────────────────
// Print Results
// ─────────────────────────────────────────────

void printResults(vector<Process> &procs)
{

    cout << "\n  "
         << left << setw(10) << "Process"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(14) << "Completion"
         << setw(14) << "Turnaround"
         << setw(12) << "Response"
         << "\n";

    cout << "  " << string(70, '-') << "\n";

    double totalTAT = 0, totalRT = 0;
    int n = procs.size();

    for (auto &p : procs)
    {

        p.turnaround = p.completion - p.arrival;
        p.response = p.firstRun - p.arrival;

        totalTAT += p.turnaround;
        totalRT += p.response;

        cout << "  "
             << left << setw(10) << ("P" + to_string(p.id))
             << setw(10) << p.arrival
             << setw(10) << p.burst
             << setw(14) << p.completion
             << setw(14) << p.turnaround
             << setw(12) << p.response
             << "\n";
    }

    cout << "\n  Avg Turnaround Time : " << fixed << setprecision(2)
         << totalTAT / n;

    cout << "\n  Avg Response Time   : " << fixed << setprecision(2)
         << totalRT / n;

    cout << "\n\n";
}

// ─────────────────────────────────────────────
// FIFO
// ─────────────────────────────────────────────

void fifo(vector<Process> procs)
{

    cout << "\n============================\n";
    cout << "  FIFO Scheduling\n";
    cout << "============================\n";

    sort(procs.begin(), procs.end(),
         [](Process a, Process b)
         {
             return a.arrival < b.arrival;
         });

    int time = 0;

    for (auto &p : procs)
    {
        if (time < p.arrival)
            time = p.arrival;

        if (p.firstRun == -1)
            p.firstRun = time;

        time += p.burst;
        p.completion = time;
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// SJF
// ─────────────────────────────────────────────

void sjf(vector<Process> procs)
{

    cout << "\n============================\n";
    cout << "  SJF Scheduling\n";
    cout << "============================\n";

    int n = procs.size();
    vector<bool> done(n, false);
    int time = 0, completed = 0;

    while (completed < n)
    {

        int idx = -1;

        for (int i = 0; i < n; i++)
        {
            if (!done[i] && procs[i].arrival <= time)
            {
                if (idx == -1 || procs[i].burst < procs[idx].burst)
                    idx = i;
            }
        }

        if (idx == -1)
        {
            time++;
            continue;
        }

        if (procs[idx].firstRun == -1)
            procs[idx].firstRun = time;

        time += procs[idx].burst;
        procs[idx].completion = time;

        done[idx] = true;
        completed++;
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// STCF
// ─────────────────────────────────────────────

void stcf(vector<Process> procs)
{

    cout << "\n============================\n";
    cout << "  STCF Scheduling\n";
    cout << "============================\n";

    int n = procs.size();
    vector<int> remaining(n);

    for (int i = 0; i < n; i++)
        remaining[i] = procs[i].burst;

    int time = 0, completed = 0;

    while (completed < n)
    {

        int idx = -1;

        for (int i = 0; i < n; i++)
        {
            if (procs[i].arrival <= time && remaining[i] > 0)
            {
                if (idx == -1 || remaining[i] < remaining[idx])
                    idx = i;
            }
        }

        if (idx == -1)
        {
            time++;
            continue;
        }

        if (procs[idx].firstRun == -1)
            procs[idx].firstRun = time;

        remaining[idx]--;
        time++;

        if (remaining[idx] == 0)
        {
            procs[idx].completion = time;
            completed++;
        }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// Round Robin (FIXED)
// ─────────────────────────────────────────────

void roundRobin(vector<Process> procs, int quantum)
{

    cout << "\n============================\n";
    cout << "  Round Robin Scheduling\n";
    cout << "============================\n";

    int n = procs.size();
    vector<int> remaining(n);

    for (int i = 0; i < n; i++)
        remaining[i] = procs[i].burst;

    queue<int> q;
    vector<bool> inQueue(n, false);

    int time = 0, completed = 0;

    while (completed < n)
    {

        for (int i = 0; i < n; i++)
        {
            if (!inQueue[i] && remaining[i] > 0 && procs[i].arrival <= time)
            {
                q.push(i);
                inQueue[i] = true;
            }
        }

        if (q.empty())
        {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();
        inQueue[i] = false; // ✅ FIXED

        if (procs[i].firstRun == -1)
            procs[i].firstRun = time;

        int run = min(quantum, remaining[i]);
        remaining[i] -= run;
        time += run;

        for (int j = 0; j < n; j++)
        {
            if (!inQueue[j] && remaining[j] > 0 &&
                procs[j].arrival <= time)
            {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if (remaining[i] == 0)
        {
            procs[i].completion = time;
            completed++;
        }
        else
        {
            q.push(i);
            inQueue[i] = true;
        }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// Lottery Scheduling (FIXED)
// ─────────────────────────────────────────────

void lottery(vector<Process> procs, int quantum)
{

    cout << "\n============================\n";
    cout << "  Lottery Scheduling\n";
    cout << "============================\n";

    srand(42);

    int n = procs.size();
    vector<int> remaining(n);

    for (int i = 0; i < n; i++)
    {
        remaining[i] = procs[i].burst;
        procs[i].firstRun = -1;
    }

    int time = 0, completed = 0;

    while (completed < n)
    {

        vector<int> eligible;
        int totalTickets = 0;

        for (int i = 0; i < n; i++)
        {
            if (procs[i].arrival <= time && remaining[i] > 0)
            {
                eligible.push_back(i);
                totalTickets += procs[i].tickets;
            }
        }

        if (eligible.empty())
        {
            time++;
            continue;
        }

        int win = rand() % totalTickets;

        int sum = 0;
        int selected = -1;

        for (int idx : eligible)
        {
            sum += procs[idx].tickets;
            if (win < sum)
            {
                selected = idx;
                break;
            }
        }

        if (procs[selected].firstRun == -1)
            procs[selected].firstRun = time;

        int run = min(quantum, remaining[selected]);

        remaining[selected] -= run;
        time += run;

        if (remaining[selected] == 0)
        {
            procs[selected].completion = time;
            completed++;
        }
    }

    printResults(procs);
}

// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────

int main()
{

    vector<Process> base = {
        {1, 0, 8, 4},
        {2, 1, 4, 2},
        {3, 2, 9, 1}, // ✅ fixed
        {4, 3, 5, 3}  // ✅ fixed
    };

    int quantum = 3;

    fifo(base);
    sjf(base);
    stcf(base);
    roundRobin(base, quantum);
    lottery(base, quantum);

    return 0;
}