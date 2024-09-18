#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Process
{
    int id;              // Process ID
    int burst_time;      // CPU Burst Time
    int arrival_time;    // Arrival Time
    int priority;        // Priority for Priority Scheduling
    int waiting_time;    // Waiting Time
    int turnaround_time; // Turnaround Time
    int remaining_time;  // Remaining Time for RR
};

void fcfs(struct Process processes[], int n);
void sjf(struct Process processes[], int n);
void round_robin(struct Process processes[], int n, int quantum);
void priority_scheduling(struct Process processes[], int n);
void calculate_waiting_turnaround(struct Process processes[], int n);
void displaynonpriority(struct Process processes[], int n);
void displaypriority(struct Process processes[], int n);
void calculate_avg_times(struct Process processes[], int n);

// Display the process information
void displaynonpriority(struct Process processes[], int n)
{
    printf("Process ID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burst_time, processes[i].arrival_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
}

void displaypriority(struct Process processes[], int n)
{
    printf("Process ID\tBurst Time\tArrival Time\tWaiting Time\tPriority\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burst_time, processes[i].arrival_time, processes[i].waiting_time, processes[i].priority, processes[i].turnaround_time);
    }
}

// Function to calculate waiting time and turnaround time
void calculate_waiting_turnaround(struct Process processes[], int n)
{
    for (int i = 0; i < n; i++)
    {
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}

// Function to calculate and display the average waiting and turnaround times
void calculate_avg_times(struct Process processes[], int n)
{
    float total_waiting_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < n; i++)
    {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
}

// First-Come-First-Serve (FCFS) Scheduling
void fcfs(struct Process processes[], int n)
{
    int current_time = 0;

    for (int i = 0; i < n; i++)
    {
        if (current_time < processes[i].arrival_time)
        {
            current_time = processes[i].arrival_time; // CPU waits for the process to arrive
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }

    calculate_waiting_turnaround(processes, n);
    displaynonpriority(processes, n);
    calculate_avg_times(processes, n);
}

// Shortest Job First (SJF) Scheduling
void sjf(struct Process processes[], int n)
{
    struct Process temp;
    int current_time = 0, completed = 0;
    bool found;

    while (completed != n)
    {
        found = false;
        int shortest = -1;
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0)
            {
                if (shortest == -1 || processes[i].burst_time < processes[shortest].burst_time)
                {
                    shortest = i;
                    found = true;
                }
            }
        }

        if (!found)
        {
            current_time++;
            continue;
        }

        processes[shortest].waiting_time = current_time - processes[shortest].arrival_time;
        current_time += processes[shortest].burst_time;
        processes[shortest].remaining_time = 0; // Process is completed
        completed++;
    }

    calculate_waiting_turnaround(processes, n);
    displaynonpriority(processes, n);
    calculate_avg_times(processes, n);
}

// Round-Robin (RR) Scheduling
void round_robin(struct Process processes[], int n, int quantum)
{
    int time = 0;      // Current time
    int remaining = n; // Number of processes left
    int i;

    // Set remaining time for all processes initially to burst time
    for (i = 0; i < n; i++)
    {
        processes[i].remaining_time = processes[i].burst_time;
    }

    // Continue until all processes are finished
    while (remaining > 0)
    {
        bool any_process_ran = false; // Flag to check if any process ran during this cycle

        // Traverse all processes to schedule them in Round-Robin manner
        for (i = 0; i < n; i++)
        {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= time)
            {
                // Process can run in the current time slot
                any_process_ran = true;

                if (processes[i].remaining_time > quantum)
                {
                    time += quantum;
                    processes[i].remaining_time -= quantum;
                }
                else
                {
                    // Process finishes
                    time += processes[i].remaining_time;
                    processes[i].waiting_time = time - processes[i].arrival_time - processes[i].burst_time;
                    processes[i].remaining_time = 0; // Mark process as finished
                    remaining--;
                }
            }
        }

        // If no process ran in this cycle, increment time to move forward
        if (!any_process_ran)
        {
            time++;
        }
    }

    // Calculate the turnaround times for all processes
    calculate_waiting_turnaround(processes, n);

    // Display process information
    displaynonpriority(processes, n);

    // Calculate and display average waiting and turnaround times
    calculate_avg_times(processes, n);
}

// Priority Scheduling
void priority_scheduling(struct Process processes[], int n)
{
    struct Process temp;
    // Input priorities
    for (int i = 0; i < n; i++)
    {
        printf("Enter priority for Process %d (higher number = lower priority): ", i + 1);
        scanf("%d", &processes[i].priority);
    }

    // Sort by priority, considering arrival time
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (processes[i].priority > processes[j].priority ||
                (processes[i].priority == processes[j].priority && processes[i].arrival_time > processes[j].arrival_time))
            {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // Calculate waiting time and turnaround time
    int current_time = 0;
    for (int i = 0; i < n; i++)
    {
        if (current_time < processes[i].arrival_time)
        {
            current_time = processes[i].arrival_time; // CPU waits for the process to arrive
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }

    // Now that we have waiting times, calculate turnaround times
    calculate_waiting_turnaround(processes, n);

    // Display process info
    displaypriority(processes, n);
    calculate_avg_times(processes, n);
}

int main()
{
    int n, algorithm_choice, quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    // Input process details
    for (int i = 0; i < n; i++)
    {
        processes[i].id = i + 1;
        printf("Enter burst time for Process %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
        printf("Enter arrival time for Process %d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);

        processes[i].remaining_time = processes[i].burst_time; // Set remaining time for RR
    }

    printf("\nChoose Scheduling Algorithm:\n");
    printf("1. First-Come-First-Serve (FCFS)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Round-Robin (RR)\n");
    printf("4. Priority Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &algorithm_choice);

    switch (algorithm_choice)
    {
    case 1:
        fcfs(processes, n);
        break;
    case 2:
        sjf(processes, n);
        break;
    case 3:
        printf("Enter time quantum for Round-Robin: ");
        scanf("%d", &quantum);
        round_robin(processes, n, quantum);
        break;
    case 4:
        priority_scheduling(processes, n);
        break;
    default:
        printf("Invalid choice!\n");
        return 1;
    }
    return 0;
}