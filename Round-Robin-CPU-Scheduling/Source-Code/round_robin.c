#include <stdio.h>

int main() {

    int n, quantum;

    printf("========================================\n");
    printf("      ROUND ROBIN CPU SCHEDULING\n");
    printf("========================================\n\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int arrival[n];
    int burst[n];
    int remaining[n];
    int completion[n];
    int turnaround[n];
    int waiting[n];
    int added[n];

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &arrival[i], &burst[i]);

        remaining[i] = burst[i];
        completion[i] = 0;
        added[i] = 0;
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);

    if (quantum <= 0) {
        printf("Time Quantum must be greater than 0.\n");
        return 1;
    }

    // Ready queue
    int queue[n * 10];
    int front = 0;
    int rear = 0;

    int time = 0;
    int completed = 0;

    printf("\n========================================\n");
    printf("              GANTT CHART\n");
    printf("========================================\n");

    printf("0");

    while (completed < n) {

        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (arrival[i] <= time &&
                remaining[i] > 0 &&
                added[i] == 0) {

                queue[rear++] = i;
                added[i] = 1;
            }
        }

        // CPU idle if queue is empty
        if (front == rear) {

            int nextArrival = -1;

            for (int i = 0; i < n; i++) {
                if (remaining[i] > 0 && added[i] == 0) {

                    if (nextArrival == -1 ||
                        arrival[i] < nextArrival) {
                        nextArrival = arrival[i];
                    }
                }
            }

            if (nextArrival > time) {
                printf(" -- IDLE -- %d", nextArrival);
                time = nextArrival;
            }

            continue;
        }

        // Get first process from queue
        int current = queue[front++];

        int executionTime;

        if (remaining[current] > quantum)
            executionTime = quantum;
        else
            executionTime = remaining[current];

        // Execute process
        printf(" -- P%d -- ", current + 1);

        time += executionTime;
        remaining[current] -= executionTime;

        printf("%d", time);

        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (arrival[i] <= time &&
                remaining[i] > 0 &&
                added[i] == 0) {

                queue[rear++] = i;
                added[i] = 1;
            }
        }

        // Process finished
        if (remaining[current] == 0) {

            completion[current] = time;
            completed++;

        } else {

            // Put process at the end of queue
            queue[rear++] = current;
        }
    }

    // Calculate results
    float totalTurnaround = 0;
    float totalWaiting = 0;

    printf("\n\n========================================\n");
    printf("          PROCESS RESULTS\n");
    printf("========================================\n\n");

    printf("%-10s %-8s %-8s %-8s %-8s %-8s\n",
           "Process", "AT", "BT", "CT", "TAT", "WT");

    printf("----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {

        turnaround[i] =
            completion[i] - arrival[i];

        waiting[i] =
            turnaround[i] - burst[i];

        totalTurnaround += turnaround[i];
        totalWaiting += waiting[i];

        printf("P%-9d %-8d %-8d %-8d %-8d %-8d\n",
               i + 1,
               arrival[i],
               burst[i],
               completion[i],
               turnaround[i],
               waiting[i]);
    }

    printf("\n========================================\n");
    printf("              FINAL RESULTS\n");
    printf("========================================\n");

    printf("Average Turnaround Time = %.2f\n",
           totalTurnaround / n);

    printf("Average Waiting Time    = %.2f\n",
           totalWaiting / n);

    printf("========================================\n");

    return 0;
}
