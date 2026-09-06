/*
tsp-backtracking Branch & Bound (Reduced Matrix Method) - Master Problem
------------------------------------------------------------
What this program solves:
Solves Travelling Salesman exactly with the classic REDUCED MATRIX
branch & bound: at every node, reduce rows/columns (subtract each
row/col's minimum) - the reduction total is a LOWER BOUND on any
tour through that state. Explore the most promising node first;
prune nodes whose bound >= best complete tour found.

Real life example:
Circuit-board drilling and delivery routing: visit every point
with minimum travel - exact answers for medium-sized instances.

DIAGRAM (row/column reduction gives a LOWER BOUND on any tour):
  subtract each row's min, then each column's min:
  sum of reductions = guaranteed minimum tour cost
  expand the most promising partial path; prune nodes whose bound
  >= best complete tour found.

BEGINNER EXPLANATION:
Reduction: a row full of costs must eventually pay at least its
smallest entry - so subtract that from every row (same for
columns) and ADD the subtracted amounts to the bound. Setting an
edge to infinity forbids it. Choosing edge i->j: row i and column
j become forbidden (out-degree 1, in-degree 1), and j->i closes
the premature cycle when only 2 cities remain. The root bound =
sum of all reductions. Children inherit parent matrix + their own
reductions. Best-first (lowest bound) search + prune vs current
best = exact answer, usually far faster than n! enumeration.

SUPER SIMPLE EXAMPLE (4 cities):
Root matrix reduces by row mins (say 10+2+3+4=19) and column mins
(say 5) -> root bound 24. A child choosing 0->2 reduces further
(say by 7) -> bound 31. If a complete tour of cost 29 exists, the
31-child is PRUNED instantly - that is the power.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Asymmetric distances fully supported (no symmetry assumed).
- All-zero row after reductions: fine, zero is a valid min.
- A node whose matrix has an infinity in every row position of a
  column: infeasible - its bound is INF and it dies.
- n = 2: the tour is 0->1->0 (forced); the special "avoid
  premature cycle" rule matters only for n >= 3.
- Priority queue order can change which complete tours are found
  early (affects pruning speed, never correctness).

DRY RUN (n = 4, all-ones distances):
Every row min = 1, column mins after = 0 -> root bound = 4.
Best-first reaches a leaf 0->1->2->3->0 with cost 4; bound 4
matches -> answer 4 immediately (tiny example, no pruning needed
but bound == optimum is visible).

TIPS:
- The three B&B ingredients: BOUND (reductions), BRANCH (edge
  choices), BEST-first ordering. Swap the bound for LP relaxation
  and you get modern tsp-backtracking solvers' skeleton.
- This is THE DAA-lab B&B problem - examiners expect the reduced
  matrix trace on paper; running this program matches that trace.
- Compare with tsp-bitmask-dp.c (folder 19): same exact problem,
  opposite memory/time profiles (2^n table vs path tree).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 12
#define INF 1000000

typedef struct {
    int reduced[MAX_N][MAX_N];
    int original[MAX_N][MAX_N];
    int cost;
    int realCost;
    int vertex;
    int path[MAX_N + 1];
    int pathLen;
    int visitedMask;
} Node;

Node heap[5000];
int heapSize = 0;

/*
 * heapPush / heapPop - Min-heap on cost.
 */
void heapPush(Node node) {
    if (heapSize >= 5000) {
        return;
    }
    int i = heapSize;
    heapSize = heapSize + 1;
    heap[i] = node;
    while (i > 0 && heap[(i - 1) / 2].cost > heap[i].cost) {
        Node t = heap[(i - 1) / 2];
        heap[(i - 1) / 2] = heap[i];
        heap[i] = t;
        i = (i - 1) / 2;
    }
}

Node heapPop(void) {
    Node top = heap[0];
    heapSize = heapSize - 1;
    heap[0] = heap[heapSize];
    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < heapSize && heap[l].cost < heap[smallest].cost) {
            smallest = l;
        }
        if (r < heapSize && heap[r].cost < heap[smallest].cost) {
            smallest = r;
        }
        if (smallest == i) {
            break;
        }
        Node t = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = t;
        i = smallest;
    }
    return top;
}

/*
 * reduceMatrix - Subtracts row/col minimums; returns the total.
 */
int reduceMatrix(int m[MAX_N][MAX_N], int n) {
    int reduction = 0;

    for (int i = 0; i < n; i++) {
        int rowMin = INF;
        for (int j = 0; j < n; j++) {
            if (m[i][j] < rowMin) {
                rowMin = m[i][j];
            }
        }
        if (rowMin != INF && rowMin > 0) {
            for (int j = 0; j < n; j++) {
                if (m[i][j] != INF) {
                    m[i][j] = m[i][j] - rowMin;
                }
            }
            reduction = reduction + rowMin;
        }
    }

    for (int j = 0; j < n; j++) {
        int colMin = INF;
        for (int i = 0; i < n; i++) {
            if (m[i][j] < colMin) {
                colMin = m[i][j];
            }
        }
        if (colMin != INF && colMin > 0) {
            for (int i = 0; i < n; i++) {
                if (m[i][j] != INF) {
                    m[i][j] = m[i][j] - colMin;
                }
            }
            reduction = reduction + colMin;
        }
    }
    return reduction;
}

int main() {
    int n;
    int dist[MAX_N][MAX_N];

    printf("tsp-backtracking Branch & Bound (Reduced Matrix)\n\n");

    printf("Number of cities (2-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 2 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter the distance matrix (%d values per row):\n", n);
    for (int i = 0; i < n; i++) {
        printf("  Row %d: ", i);
        for (int j = 0; j < n; j++) {
            scanf("%d", &dist[i][j]);
            if (i == j) {
                dist[i][j] = INF;
            }
        }
    }

    /* STEP: root node with reduced matrix */
    Node root;
    memcpy(root.reduced, dist, sizeof(dist));
    memcpy(root.original, dist, sizeof(dist));
    root.cost = reduceMatrix(root.reduced, n);
    root.realCost = 0;
    root.vertex = 0;
    root.path[0] = 0;
    root.pathLen = 1;
    root.visitedMask = 1;
    heapPush(root);
    printf("Root bound: %d\n", root.cost);

    int best = INF;
    Node bestNode;
    bestNode.pathLen = 0;

    /* STEP: best-first search with pruning */
    while (heapSize > 0) {
        Node node = heapPop();
        if (node.cost >= best) {
            continue;
        }

        if (node.pathLen == n) {
            /* complete tours arrive here only if pushed as such */
            if (node.realCost < best) {
                best = node.realCost;
                bestNode = node;
            }
            continue;
        }

        /* exactly one city remains: go there, then close to 0 */
        if (node.pathLen == n - 1) {
            int last = -1;
            for (int x = 0; x < n; x++) {
                if (!(node.visitedMask & (1 << x))) {
                    last = x;
                    break;
                }
            }
            if (last == -1) {
                continue;
            }
            if (node.reduced[node.vertex][last] == INF) {
                continue;
            }
            /* real cost uses the ORIGINAL matrix */
            int total = node.realCost +
                        node.original[node.vertex][last] +
                        node.original[last][0];
            if (total < best) {
                best = total;
                Node done = node;
                done.path[node.pathLen] = last;
                done.pathLen = node.pathLen + 1;
                bestNode = done;
            }
            continue;
        }

        for (int v = 1; v < n; v++) {
            if (node.visitedMask & (1 << v)) {
                continue;
            }
            if (node.reduced[node.vertex][v] == INF) {
                continue;
            }

            Node child = node;
            int edgeCost = node.reduced[node.vertex][v];
            int realEdge = node.original[node.vertex][v];
            child.reduced[node.vertex][v] = INF;
            child.reduced[v][node.vertex] = INF;
            /* row of node.vertex: no more outgoing */
            for (int j = 0; j < n; j++) {
                child.reduced[node.vertex][j] = INF;
            }
            /* column of v: no more incoming */
            for (int i = 0; i < n; i++) {
                child.reduced[i][v] = INF;
            }

            int reduction = reduceMatrix(child.reduced, n);
            child.cost = node.cost + edgeCost + reduction;
            child.realCost = node.realCost + realEdge;
            if (child.cost >= best) {
                continue;
            }
            child.vertex = v;
            child.path[node.pathLen] = v;
            child.pathLen = node.pathLen + 1;
            child.visitedMask = node.visitedMask | (1 << v);
            heapPush(child);
        }
    }

    if (best == INF) {
        printf("No complete tour exists\n");
        return 0;
    }

    printf("\nOptimal tour cost: %d\n", best);
    printf("Tour: 0");
    for (int i = 1; i < bestNode.pathLen; i++) {
        printf(" -> %d", bestNode.path[i]);
    }
    printf(" -> 0\n");

    return 0;
}
