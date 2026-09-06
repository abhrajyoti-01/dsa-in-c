/*
0/1 Knapsack using Dynamic Programming
--------------------------------------
What this program solves:
This program finds the maximum profit when each item can either be taken fully
or not taken at all. Here, fractions are not allowed.

Real life example:
Imagine a student is packing a school bag for a trip. The bag can carry only a
limited weight. The student must decide which full items to carry: water bottle,
calculator, lunch box, jacket. The student cannot take half a calculator or half
a bottle. So for each item there are only two choices: take it or leave it.

DIAGRAM (DP table for capacity 5, items (12,2) (10,1) (20,3)):
           capacity j ->  0    1    2    3    4    5
  item 0 (nothing)       | 0 | 0 | 0 | 0 | 0 | 0 |
  item 1 (12,2)          | 0 | 0 |12 |12 |12 |12 |
  item 2 (10,1)          | 0 |10 |12 |22 |22 |22 |
  item 3 (20,3)          | 0 |10 |12 |22 |30 |32 |  <- answer dp[3][5]=32

Each cell = max profit with those items and that capacity.
Cell (3,5): max( dp[2][5]=22,              (skip item 3)
                 20 + dp[2][2]=20+12=32 )  (take item 3) = 32.

How the process works:
1. Build a table where dp[i][j] means best profit using first i items and capacity j.
2. If the current item is heavier than the current capacity, it cannot be taken.
3. Otherwise compare two choices:
   a) do not take the item
   b) take the item and add the best answer for the remaining capacity
4. Store the better value in the table.
5. The final answer is stored in dp[n][capacity].

Pseudo code:
START
Read number of items and knapsack capacity
Read profit and weight of each item
Create DP table
For each item i
    For each capacity j
        If item does not fit
            Copy value from previous row
        Else
            Take maximum of:
                excluding the item
                including the item + previous best for remaining capacity
Print final answer from DP table
END

Sample input and dry run:
Capacity = 5
Items: (profit, weight) = (12,2), (10,1), (20,3)
At each table cell, the program decides whether taking the current item gives
better profit than skipping it.
For capacity 5, taking items with profits 12 and 20 gives total profit 32.
So the final answer stored in the table becomes 32.

Main logic to understand:
Dynamic programming solves many small subproblems first and uses those answers
to solve larger ones efficiently.
*/
#include <stdio.h>

#define MAX 50

int main() {
    int profit[MAX], weight[MAX], dp[MAX][MAX];
    int n, capacity, i, j;

    printf("0/1 Knapsack using Dynamic Programming\n");
    printf("Enter the number of items: ");
    scanf("%d", &n);
    printf("Enter the capacity of the knapsack: ");
    scanf("%d", &capacity);

    for (i = 1; i <= n; i++) {
        printf("Enter profit for item %d: ", i);
        scanf("%d", &profit[i]);
        printf("Enter weight for item %d: ", i);
        scanf("%d", &weight[i]);
    }

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= capacity; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (weight[i] <= j) {
                if (profit[i] + dp[i - 1][j - weight[i]] > dp[i - 1][j]) {
                    dp[i][j] = profit[i] + dp[i - 1][j - weight[i]];
                } else {
                    dp[i][j] = dp[i - 1][j];
                }
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    printf("Maximum profit: %d\n", dp[n][capacity]);
    return 0;
}

