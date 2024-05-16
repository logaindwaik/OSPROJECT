#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// idintify variabel (iv)
struct iv {
    int pross_id;
    int arrive_time;
    int total_burst_time;
    int remining_cpu_burst;
    int  finish_time ;
   
    int wating_time;
    int turnaround_time;
};

// for shortest remaining time

struct remaining_time {
    bool operator()(const iv& p1, const iv& p2) {
        return p1.remining_cpu_burst > p2.remining_cpu_burst;
    }
};

// Function to read processes from file
vector<iv> readfile (const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    vector<iv> processes;
    int pross_id, arrive_time, burst_time1;
    while (infile >> pross_id >> arrive_time) {
        iv process;
        process.pross_id = pross_id;
        process.arrive_time = arrive_time;
        process total_burst_time = 0;
        while (infile >> burst_time1 && burst_time1 != -1) {
            process total_burst_time += burst_time1;
        }
        process.remining_cpu_burst = process.total_burst_time;
        processes.push_back(process);
    }
    infile.close();

    if (processes.empty()) {
        cerr << "file eroor  " << filename << "  not contain data." << endl;
        exit(EXIT_FAILURE);
    }

    return processes;
}

// FCFS 


void FCFS(vector<iv>& processes, int swithtime) {
    int currentTime = 0;
    for (auto& process : processes) {
        if (process.arrive_time > currentTime) {
            currentTime = process.arrive_time;
        }
        process.wating_time = currentTime - process.arrive_time;
        process.finish_time= currentTime + process.total_burst_time;
        process.turnaround_time = process.finish_time- process.arrive_time;
        currentTime += process.total_burst_time + swithtime;
    }
}

// SRT 


void SRT(vector<iv>& processes, int swithtime) {
    priority_queue<iv, vector<iv>, remaining_time> readyQueue;
    int currentTime = 0;
    size_t prosessindexe = 0;
    while (prosessindexe < processes.size() || !readyQueue.empty()) {
        while (prosessindexe < processes.size() && processes[prosessindexe].arrive_time <= currentTime) {
            readyQueue.push(processes[prosessindexe++]);
        }
        if (!readyQueue.empty()) {
            iv curentprosses = readyQueue.top();
            readyQueue.pop();
            curentprosses.remining_cpu_burst--;
            if (curentprosses.remining_cpu_burst == 0) {
                curentprosses.finish_time= currentTime + 1;
                curentprosses.turnaround_time = curentprosses.finish_time- curentprosses.arrive_time;
                curentprosses.wating_time = curentprosses.turnaround_time - curentprosses.total_burst_time;
            }
            else {
                readyQueue.push(curentprosses);
            }
        }
        currentTime++;
    }
}

// RR Scheduler


void RR(vector<iv>& processes, int time_slice, int swithtime) {
    queue<iv> readyQueue;

    int currentTime = 0;

    long long prosessindexe = 0;

    while (prosessindexe < processes.size() || !readyQueue.empty()) {
        while (prosessindexe < processes.size() && processes[prosessindexe].arrive_time <= currentTime) {
            readyQueue.push(processes[prosessindexe++]);
        }
        if (!readyQueue.empty()) {
            iv curentprosses = readyQueue.front();
            readyQueue.pop();
            int slice = min(time_slice, curentprosses.remining_cpu_burst);
            curentprosses.remining_cpu_burst -= slice;
            if (curentprosses.remining_cpu_burst == 0) {
                curentprosses.finish_time= currentTime + slice;
                curentprosses.turnaround_time = curentprosses.finish_time- curentprosses.arrive_time;
                curentprosses.wating_time = curentprosses.turnaround_time - curentprosses.total_burst_time;
            }
            else {
                readyQueue.push(curentprosses);
            }
            currentTime += slice + swithtime;
        }
        else {
            currentTime++;
        }
    }
}

// function to calculate CPU utilization
double calculateCPUUtilization(const vector<iv>& processes) {
    int total_burst_time = 0;
    for (const auto& process : processes) {
        total_burst_time += process.total_burst_time;
    }
    int totalTime = processes.back().finishTime;
    return (static_cast<double>(total_burst_time) / totalTime) * 100;
}

// Function to display results for each scheduling algorithm

void displayResults(const vector<iv>& processes) {


    // Display Gantt chart
    cout << "Gantt Chart:" << endl;
    for (const auto& process : processes) {
        cout << "Process " << process.pross_id << ": [" << process.arrive_time << "-" << process.finish_time<< "] ";
    }
    cout << endl;

    // Display finish time for each process


    cout << "Finish time for each process:" << endl;
    for (const auto& process : processes) {
        cout << "Process " << process.pross_id << ": " << process.finish_time<< endl;
    }

    // Display waiting time for each process

    cout << "Waiting time for each process:" << endl;
    for (const auto& process : processes) {
        cout << "Process " << process.pross_id << ": " << process.wating_time << endl;
    }

    // Display turnaround time for each process

    cout << "Turnaround time for each process:" << endl;
     for (const auto& process : processes) {                                                                                      
        cout << "Process " << process.pross_id << ": " << process.turnaround_time << endl;
    }

    // Display CPU utilization

    double cpuUtilization = calculateCPUUtilization(processes);
    cout << "CPU Utilization: " << cpuUtilization << "%" << endl;
}

int main() {
   
    string filename = "input.txt";
    int swithtime = 1; 
    int time_slice = 2; 

    vector<iv> processes = readfile (filename);

    // FCFS
    vector<iv> firstcomefirstserchprosses = processes;
    FCFS(firstcomefirstserchprosses, swithtime);
    cout << "first come first serch Results:" << endl;
    displayResults(firstcomefirstserchprosses);
    cout << endl;

    // SRT
    vector<iv> shortremaingtimeprosses = processes;
    SRT(shortremaingtimeprosses, swithtime);
    cout << "short remaing time Results:" << endl;
    displayResults(shortremaingtimeprosses);
    cout << endl;

    // RR
    vector<iv> roundroubenprosses = processes;
    RR(roundroubenprosses, time_slice, swithtime);
    cout << "round rouben Results:" << endl;
    displayResults(roundroubenprosses);

    return 0;
}





