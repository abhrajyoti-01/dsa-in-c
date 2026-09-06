/*
Travelling Salesman Problem
---------------------------
What this program solves:
This program finds the minimum tour cost needed to visit every city exactly once
and then return to the starting city.

Real life example:
Imagine a delivery agent who must deliver parcels to every branch office and come
back to the main office. If the agent chooses a poor route, fuel cost becomes high.
So the goal is to try possible visiting orders and keep the cheapest complete tour.

DIAGRAM (try every unvisited city next; keep the best total):
  0 -> 1 -> 2 -> 3 -> 0   cost 10+20+15+8 = 53
  0 -> 1 -> 3 -> 2 -> 0   cost 10+15+20+8 = 53? trace prints best
  prune when the partial cost already exceeds the best found.

How the process works:
1. Start from city 1.
2. Mark the current city as visited.
3. Recursively move to every possible unvisited city.
4. When all cities are visited, add the cost of returning to the start.
5. Compare all complete tour costs and keep the minimum one.

Pseudo code:
START
Read number of cities and cost matrix
Mark starting city as visited
Recursively try every unvisited next city
    Add travel cost and continue
When all cities are visited
    Add cost to return to start
Keep the minimum among all complete tours
Print minimum tour cost or no tour
END

Sample input and dry run:
Suppose there are 4 cities.
The program may try routes like 1 -> 2 -> 3 -> 4 -> 1 and 1 -> 3 -> 2 -> 4 -> 1.
For each route, it adds the travel costs step by step.
After checking all valid complete tours, it keeps the tour with the smallest total cost.
This is why the method is easy to understand but becomes expensive for larger graphs.

Main logic to understand:
This program uses recursion and backtracking. It explores one route, comes back,
tries another route, and finally keeps the best answer.
*/
#include <stdio.h>

#define MAX 10
#define INF 999999

int cost[MAX][MAX];
int visited[MAX];
int n;

int tsp(int city, int count, int totalCost) {
    int i;
    int minCost = INF;
    int currentCost;

    if (count == n) {
        if (cost[city][1] != 0) {
            return totalCost + cost[city][1];
        }
        return INF;
    }

    for (i = 1; i <= n; i++) {
        if (!visited[i] && cost[city][i] != 0) {
            visited[i] = 1;
            currentCost = tsp(i, count + 1, totalCost + cost[city][i]);
            if (currentCost < minCost) {
                minCost = currentCost;
            }
            visited[i] = 0;
        }
    }

    return minCost;
}

int main() {
    int i, j, answer;

    printf("Travelling Salesman Problem\n");
    printf("Enter the number of cities: ");
    scanf("%d", &n);

    printf("Enter the cost adjacency matrix:\n");
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }

    visited[1] = 1;
    answer = tsp(1, 1, 0);

    if (answer >= INF) {
        printf("No tour exists.\n");
    } else {
        printf("Minimum tour cost: %d\n", answer);
    }

    return 0;
}

