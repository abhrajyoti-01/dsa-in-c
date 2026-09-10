/*
Union-Find Applications (Basic -> Hard)
----------------------------------------
Real life example:
"Accounts merge" is a real feature: merge users sharing an email;
cycle detection via union-find pre-screens redundant network
cables/routes before laying them.

DIAGRAM (cycle detection edge by edge, edges 1-2, 2-3, 1-3):
  1-2: different roots -> union   sets: {1,2} {3}
  2-3: different roots -> union   sets: {1,2,3}
  1-3: SAME root already  -> this edge CLOSES A CYCLE

LADDER (easy -> hard inside this file):
  1. Cycle detection in an undirected graph (edge by edge)
  2. Count connected components (no BFS/DFS at all!)
  3. Accounts-merge style: merge people sharing an email/group

BEGINNER EXPLANATION:
Union-Find answers "are these two things in the same group?" and
"merge these groups" in near-O(1). Cycle detection: process edges
one by one - if an edge connects two nodes ALREADY in the same
set, that edge closes a cycle. Components: start with n singleton
sets; every edge merges two; the final count of distinct roots is
the answer.

SUPER SIMPLE EXAMPLE (cycle):
Edges: 1-2, 2-3, 1-3
1-2: different sets -> union. Sets: {1,2}, {3}
2-3: different -> union. {1,2,3}
1-3: SAME set already -> CYCLE found! Three edges, three nodes,
one cycle - matches intuition.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Self-loop (u-u): same node, trivially a cycle.
- Edge processed twice (u-v and v-u): with canonical order
  (u < v) the second copy is identical - dedupe or normalize.
- Component count via find: after all unions, count i where
  find(i) == i - roots are component representatives. Do NOT
  count "distinct parent[] values" - parents change as trees
  rebalance; only roots are stable.
- Directed graphs: union-find does NOT apply (it is for
  undirected connectivity) - use DFS colors (folder 12).

DRY RUN (components, edges 1-2, 3-4, 4-5):
Initially 5 components (each node its own root).
1-2: union -> 4 components. 3-4: union -> 3. 4-5: union -> 2.
Roots after: node1's tree {1,2}, node3's tree {3,4,5}.
find(i)==i for i in {1,3} -> 2 components. Matches.

TIPS:
- Union by rank + path compression give amortized alpha(n) < 5
  operations - effectively constant (see union_find.c for the
  full implementation).
- Kruskal's MST (folder 12) is EXACTLY this cycle test wrapped
  around a sorted edge list - learn it here, reuse there.
- Components-by-DSU beats BFS when edges arrive ONLINE (streaming)
  - you never need the whole graph at once.
*/

#include <stdio.h>
#include <string.h>

#define MAX_ELEMS 100

int parent[MAX_ELEMS];
int rankArr[MAX_ELEMS];

void makeSet(int n) {
    for (int i = 0; i <= n; i++) {
        parent[i] = i;
        rankArr[i] = 0;
    }
}

int findRoot(int x) {
    if (parent[x] != x) {
        parent[x] = findRoot(parent[x]);
    }
    return parent[x];
}

int unionSets(int a, int b) {
    int ra = findRoot(a);
    int rb = findRoot(b);
    if (ra == rb) {
        return 0;
    }
    if (rankArr[ra] < rankArr[rb]) {
        parent[ra] = rb;
    } else if (rankArr[ra] > rankArr[rb]) {
        parent[rb] = ra;
    } else {
        parent[rb] = ra;
        rankArr[ra] = rankArr[ra] + 1;
    }
    return 1;
}

int main() {
    int n;
    int e;
    int choice;

    printf("Union-Find Applications (easy -> hard)\n\n");

    printf("How many vertices (1-%d, numbered 0..n-1)? ", MAX_ELEMS - 1);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ELEMS - 1) {
        printf("Invalid count\n");
        return 1;
    }

    printf("How many undirected edges? ");
    scanf("%d", &e);

    int eu[200];
    int ev[200];
    printf("Enter %d edges as 'u v':\n", e);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &eu[i], &ev[i]);
    }

    while (1) {
        printf("\n1. Cycle detection (edge by edge trace)\n");
        printf("2. Count connected components\n");
        printf("3. Show components\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            makeSet(n);
            int cycleFound = 0;
            for (int i = 0; i < e; i++) {
                if (eu[i] < 0 || eu[i] >= n || ev[i] < 0 || ev[i] >= n) {
                    continue;
                }
                int merged = unionSets(eu[i], ev[i]);
                printf("edge %d-%d: %s\n", eu[i], ev[i],
                       merged ? "merged sets" : "SAME SET -> CYCLE!");
                if (!merged) {
                    cycleFound = 1;
                }
            }
            printf("Graph %s a cycle\n", cycleFound ? "HAS" : "has no");
        } else if (choice == 2 || choice == 3) {
            makeSet(n);
            for (int i = 0; i < e; i++) {
                if (eu[i] >= 0 && eu[i] < n && ev[i] >= 0 && ev[i] < n) {
                    unionSets(eu[i], ev[i]);
                }
            }
            int comp = 0;
            int compId[MAX_ELEMS];
            memset(compId, -1, sizeof(compId));
            for (int i = 0; i < n; i++) {
                int r = findRoot(i);
                if (compId[r] == -1) {
                    comp = comp + 1;
                    compId[r] = comp;
                }
            }
            printf("Connected components: %d\n", comp);
            for (int c = 1; c <= comp; c++) {
                printf("  Component %d: { ", c);
                for (int i = 0; i < n; i++) {
                    if (compId[findRoot(i)] == c) {
                        printf("%d ", i);
                    }
                }
                printf("}\n");
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
