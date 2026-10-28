/*
2-SAT - Satisfying "A or B" Constraints with an Implication Graph (Master)
-------------------------------------------------------------------------
PROBLEM DEFINITION:
Given n boolean variables and m clauses of the form (a OR b), where a
and b are variables or their negations, find an assignment that makes
every clause true, or prove that none exists. General SAT is
NP-complete, but with at most TWO literals per clause the problem is
solvable in linear time through strongly connected components.

REAL-LIFE EXAMPLE:
Scheduling with pairwise rules: "the meeting is in the morning OR the
review is in the afternoon", "Asha and Ravi cannot both present".
Placing labels on a map so that no two labels overlap (each label has
two candidate positions) is another classic 2-SAT instance.

DIAGRAM (one clause becomes two implications):
  (a OR b)  means  "if a is false then b must be true" and vice versa:
        !a ----> b          !b ----> a
  Variable x is impossible if  x ->...-> !x  AND  !x ->...-> x,
  i.e. x and !x are in the same strongly connected component.

PSEUDO CODE:
  For each clause (a OR b): add edges !a -> b and !b -> a
  Run Tarjan's SCC on the 2n literal nodes
  For each variable x:
      if comp[x] == comp[!x]: UNSATISFIABLE
  Tarjan numbers SCCs in reverse topological order, so
      x = true  if comp[x] < comp[!x]   (x's SCC is "later" in the DAG)
  (setting the literal that comes later in topological order to true
   never forces anything false.)

SAMPLE INPUT & DRY RUN:
  3 4
  1 2
  -1 3
  -2 -3
  1 -3
  Clauses: (x1 OR x2) (!x1 OR x3) (!x2 OR !x3) (x1 OR !x3)
  Implications: !x1->x2  !x2->x1   x1->x3  !x3->!x1
                x2->!x3  x3->!x2   !x1->!x3  x3->x1
  Tarjan from x1: x1 -> x3 -> !x2 -> x1 closes {x1, x3, !x2} = SCC 0;
  then {!x1, x2, !x3} = SCC 1.
  x1: comp 0 < 1 -> true;  x2: comp 1 > 0 -> false;  x3: 0 < 1 -> true
  Check: (T or F) (F or T) (T or F) (T or F) -> 4 of 4 clauses satisfied

SPECIAL CASES:
  - A unit clause "x must be true" is written (x OR x).
  - (x OR x) together with (!x OR !x) is unsatisfiable: x <-> !x.
  - A variable that appears in no clause comes out true (its x node is
    visited first, so its singleton SCC closes first); either value
    would work.
  - The program re-checks every clause against the final assignment.
  - 1 <= n <= 1000 variables, 0 <= m <= 10000 clauses; a literal is
    i (x_i) or -i (NOT x_i) for 1 <= i <= n.

COMPLEXITY:
  O(n + m) time and space: 2n nodes, 2m edges, one Tarjan pass.

TIPS:
  The modelling step is the hard part: turn every rule into clauses of
  two literals ("A implies B" is (!A OR B); "not both" is (!A OR !B);
  "exactly one" is (A OR B) and (!A OR !B)). 3-SAT is NP-complete, so
  no such trick exists once a clause needs three literals.
  See tarjan-scc for the SCC engine used here.
*/

#include <stdio.h>

#define MAX_VARS 1000
#define MAX_CLAUSES 10000
#define MAX_NODES (2 * MAX_VARS)
#define MAX_EDGES (2 * MAX_CLAUSES)
#define TRACE_LIMIT 10

int variableCount;
int clauseCount;
int clauseA[MAX_CLAUSES];
int clauseB[MAX_CLAUSES];
int edgeFrom[MAX_EDGES];
int edgeTo[MAX_EDGES];
int edgeCount;
int start[MAX_NODES + 1];
int adjacency[MAX_EDGES];
int disc[MAX_NODES];
int low[MAX_NODES];
int onStack[MAX_NODES];
int component[MAX_NODES];
int stack[MAX_NODES];
int top;
int timer;
int componentCount;

/* Literal i (1-based) is node 2(i-1); literal -i is node 2(i-1)+1. */
int nodeOf(int literal) {
    return literal > 0 ? 2 * (literal - 1) : 2 * (-literal - 1) + 1;
}

void printNode(int node) {
    printf(node % 2 == 0 ? "x%d" : "!x%d", node / 2 + 1);
}

void addEdge(int from, int to) {
    edgeFrom[edgeCount] = from;
    edgeTo[edgeCount] = to;
    edgeCount++;
}

void buildAdjacency(int nodes) {
    for (int i = 0; i < edgeCount; i++) start[edgeFrom[i] + 1]++;
    for (int u = 0; u < nodes; u++) start[u + 1] += start[u];
    int fill[MAX_NODES];
    for (int u = 0; u < nodes; u++) fill[u] = start[u];
    for (int i = 0; i < edgeCount; i++) adjacency[fill[edgeFrom[i]]++] = edgeTo[i];
}

void dfs(int u) {
    disc[u] = low[u] = ++timer;
    stack[top++] = u;
    onStack[u] = 1;
    for (int i = start[u]; i < start[u + 1]; i++) {
        int v = adjacency[i];
        if (disc[v] == 0) {
            dfs(v);
            if (low[v] < low[u]) low[u] = low[v];
        } else if (onStack[v] && disc[v] < low[u]) {
            low[u] = disc[v];
        }
    }
    if (low[u] == disc[u]) {
        int v;
        do {
            v = stack[--top];
            onStack[v] = 0;
            component[v] = componentCount;
        } while (v != u);
        componentCount++;
    }
}

int main(void) {
    printf("2-SAT - Implication Graph + Strongly Connected Components\n\n");
    printf("Variables n (1-%d) and clauses m (0-%d): ", MAX_VARS, MAX_CLAUSES);
    if (scanf("%d %d", &variableCount, &clauseCount) != 2 ||
        variableCount < 1 || variableCount > MAX_VARS || clauseCount < 0 ||
        clauseCount > MAX_CLAUSES) {
        printf("Invalid size\n");
        return 1;
    }
    for (int i = 0; i < clauseCount; i++) {
        printf("Clause %d: two literals (i or -i): ", i + 1);
        if (scanf("%d %d", &clauseA[i], &clauseB[i]) != 2 || clauseA[i] == 0 ||
            clauseB[i] == 0 || clauseA[i] < -variableCount ||
            clauseA[i] > variableCount || clauseB[i] < -variableCount ||
            clauseB[i] > variableCount) {
            printf("Invalid literal\n");
            return 1;
        }
        int a = nodeOf(clauseA[i]);
        int b = nodeOf(clauseB[i]);
        addEdge(a ^ 1, b);
        addEdge(b ^ 1, a);
    }
    int nodes = 2 * variableCount;
    buildAdjacency(nodes);

    if (clauseCount <= TRACE_LIMIT) {
        printf("\nImplications:");
        for (int i = 0; i < edgeCount; i++) {
            printf(i % 4 == 0 ? "\n  " : "   ");
            printNode(edgeFrom[i]);
            printf(" -> ");
            printNode(edgeTo[i]);
        }
        printf("\n");
    }

    for (int u = 0; u < nodes; u++) {
        if (disc[u] == 0) dfs(u);
    }

    if (variableCount <= TRACE_LIMIT) {
        printf("\nSCC of each literal (Tarjan order):\n");
        for (int i = 0; i < variableCount; i++) {
            printf("  x%d: %d   !x%d: %d\n", i + 1, component[2 * i], i + 1,
                   component[2 * i + 1]);
        }
    }

    for (int i = 0; i < variableCount; i++) {
        if (component[2 * i] == component[2 * i + 1]) {
            printf("\nUNSATISFIABLE: x%d and !x%d imply each other "
                   "(same SCC %d)\n", i + 1, i + 1, component[2 * i]);
            return 0;
        }
    }

    int value[MAX_VARS];
    printf("\nSATISFIABLE:");
    for (int i = 0; i < variableCount; i++) {
        value[i] = component[2 * i] < component[2 * i + 1];
        if (variableCount <= 50) {
            printf(" x%d=%s", i + 1, value[i] ? "true" : "false");
        }
    }
    printf("\n");

    int satisfied = 0;
    for (int i = 0; i < clauseCount; i++) {
        int a = clauseA[i] > 0 ? value[clauseA[i] - 1] : !value[-clauseA[i] - 1];
        int b = clauseB[i] > 0 ? value[clauseB[i] - 1] : !value[-clauseB[i] - 1];
        if (a || b) satisfied++;
    }
    printf("Clause check: %d of %d clauses satisfied\n", satisfied, clauseCount);
    return 0;
}
