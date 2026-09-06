/*
Job Sequencing with Deadlines
-----------------------------
What this program solves:
Suppose a company has many customer orders, but only one machine is available,
and each order gives a different profit. Every order must be finished before its
deadline. This program selects the jobs that give maximum total profit.

Real life example:
Imagine a print shop has 3 orders:
Job 1 -> deadline 2, profit 100
Job 2 -> deadline 1, profit 19
Job 3 -> deadline 2, profit 27
Because only one job can be done in one time slot, the smart choice is to first
consider the highest-profit job and place it as late as possible before its deadline.
That keeps earlier slots open for other jobs.

DIAGRAM (jobs (profit, deadline): J1(100,2) J2(19,1) J3(27,2)):
  sort by profit: J1 J3 J2
  J1 -> slot 2 (latest free <= 2)
  J3 -> slot 1
  J2 -> no free slot -> dropped
  schedule: J3, J1  profit = 127

How the process works:
1. Read all jobs with their deadlines and profits.
2. Sort jobs in descending order of profit.
3. For each job, check slots from its deadline backwards.
4. Put the job in the latest free slot available.
5. If no slot is free, skip that job.
6. Add the profit of every selected job.

Pseudo code:
START
Read all jobs with deadline and profit
Sort jobs by profit in descending order
For each job in sorted order
    Check slots from its deadline backwards
    If a free slot is found
        Assign the job to that slot
        Add its profit
Print selected jobs and total profit
END

Sample input and dry run:
Jobs: J1(d=2,p=100), J2(d=1,p=19), J3(d=2,p=27), J4(d=1,p=25)
After sorting by profit: J1, J3, J4, J2
Place J1 in slot 2.
Place J3 in slot 1 because slot 2 is already full.
J4 and J2 cannot be placed because slot 1 is already occupied.
Selected jobs are J3 and J1 with total profit 127.

Why this greedy logic works:
Choosing high-profit jobs first and placing them as late as possible gives the
best chance to fit more profitable jobs into the schedule.
*/
#include <stdio.h>

#define MAX 50

struct Job {
    int id;
    int deadline;
    int profit;
};

int main() {
    struct Job jobs[MAX], temp;
    int slots[MAX] = {0};
    int n, i, j, maxDeadline = 0, totalProfit = 0;

    printf("Job Sequencing with Deadlines\n");
    printf("Enter the number of jobs: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        jobs[i].id = i + 1;
        printf("Enter deadline for job %d: ", i + 1);
        scanf("%d", &jobs[i].deadline);
        printf("Enter profit for job %d: ", i + 1);
        scanf("%d", &jobs[i].profit);

        if (jobs[i].deadline > maxDeadline) {
            maxDeadline = jobs[i].deadline;
        }
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (jobs[j].profit < jobs[j + 1].profit) {
                temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < n; i++) {
        for (j = jobs[i].deadline; j >= 1; j--) {
            if (slots[j] == 0) {
                slots[j] = jobs[i].id;
                totalProfit += jobs[i].profit;
                break;
            }
        }
    }

    printf("\nSelected job order:\n");
    for (i = 1; i <= maxDeadline; i++) {
        if (slots[i] != 0) {
            printf("Slot %d -> Job %d\n", i, slots[i]);
        }
    }

    printf("Total profit: %d\n", totalProfit);
    return 0;
}

