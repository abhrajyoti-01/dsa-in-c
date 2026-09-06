/*
Bipartite Matching - Hungarian Algorithm (Kuhn's / Hopcroft-Karp lite)
----------------------------------------------------------------------
What this program solves:
Matches workers to jobs (or students to projects): worker i can do
job j only if edge (i,j) exists; each worker takes AT MOST one job,
each job goes to at most one worker. Finds the MAXIMUM number of
matched pairs.

Real life example:
Job applicants to openings, students to projects, riders to
drivers - maximum matching answers "how many pairs can we form?".

DIAGRAM (augmenting path flips matched/unmatched edges):
  applicants A B   jobs 1 2
  A-1 matched; B can only do 1 -> B-1, 1-A, A-2 chain REVERSED:
  now B-1 and A-2 matched -> matching grew by one.

BEGINNER EXPLANATION (the simplest way to think):
Speed dating: each worker on the left, jobs on the right, lines
(eligibility) between them. Try to seat each worker in turn:
- If one of their eligible jobs is FREE -> seat them (easy match).
- If all eligible jobs are TAKEN, ask each job's current worker:
  "can YOU switch to some other free job?" If yes, they switch and
  the new worker takes the job. This recursive "re-arranging chain"
  is an AUGMENTING PATH. If no chain works, this worker stays
  unmatched (for now).

SUPER SIMPLE EXAMPLE:
Workers {w0, w1}, jobs {j0, j1}
w0 eligible: j0, j1.  w1 eligible: j0.
w0 tries j0 -> free -> match (w0, j0).
w1 tries j0 -> taken by w0 -> can w0 switch to j1? Yes (free)!
Result: w1->j0, w0->j1. Both matched - max matching = 2.
Without re-arranging you'd wrongly stop at 1.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Isolated vertex (no eligibility edges): stays unmatched; the
  rest still match normally.
- The greedy order matters for the PATH taken, never for the
  final count - maximum matching is order-independent.
- Perfect matching may not exist (more workers than jobs): answer
  = min side count at most.
- Every augmenting path found increases matching by EXACTLY 1 -
  that is why the algorithm stops when no DFS succeeds.

DRY RUN (classic 6-vertex example):
Workers 0,1,2; jobs 3,4,5 (bipartition)
eligibility: 0-{3,4}, 1-{4}, 2-{3,5}
DFS(0): try 3 free -> match 0-3. matching=1
DFS(1): try 4 free -> match 1-4. matching=2
DFS(2): try 3 -> taken by 0 -> can 0 re-match? 0's options: 3(taken
        by 2? no we are mid-DFS), 4 -> taken by 1 -> can 1 rematch?
        1's only option is 4 = current visitor -> fail. So 0 fails.
        try 5 -> free -> match 2-5. matching=3
No more workers. Max matching = 3 (0-3, 1-4, 2-5).

TIPS:
- This IS max-flow on a tiny network: s->workers (cap 1),
  jobs->t (cap 1), eligibility edges cap 1. Matching value = flow.
- KÃ¶nig's theorem: max matching = minimum vertex cover (left side
  of unmatchable DFS tree). Advanced but beautiful.
- Time O(V * E) - fine for hundreds of workers; huge instances
  use Hopcroft-Karp (O(E * sqrt(V))).
*/

#include <stdio.h>
#include <string.h>

#define MAX_V 40

int adj[MAX_V][MAX_V];
int matchLeft[MAX_V];
int matchRight[MAX_V];
int visitedRight[MAX_V];
int nLeft;
int nRight;

/*
 * tryKuhn - DFS augmenting path search for worker u.
 */
int tryKuhn(int u) {
    for (int j = 0; j < nRight; j++) {
        if (adj[u][j] && !visitedRight[j]) {
            visitedRight[j] = 1;

            if (matchRight[j] == -1 || tryKuhn(matchRight[j])) {
                matchRight[j] = u;
                matchLeft[u] = j;
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    int e;
    int u;
    int j;

    printf("Bipartite Matching (Kuhn's Algorithm)\n\n");

    printf("How many workers on the LEFT (1-%d)? ", MAX_V);
    scanf("%d", &nLeft);

    printf("How many jobs on the RIGHT (1-%d)? ", MAX_V);
    scanf("%d", &nRight);

    if (nLeft < 1 || nLeft > MAX_V || nRight < 1 || nRight > MAX_V) {
        printf("Invalid counts\n");
        return 1;
    }

    printf("How many eligibility edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'worker job' (worker 0..%d, job 0..%d):\n",
           e, nLeft - 1, nRight - 1);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &j);
        if (u < 0 || u >= nLeft || j < 0 || j >= nRight) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        adj[u][j] = 1;
    }

    for (int i = 0; i < nLeft; i++) {
        matchLeft[i] = -1;
    }
    for (int j = 0; j < nRight; j++) {
        matchRight[j] = -1;
    }

    /* STEP: one DFS per worker, in order */
    int matching = 0;
    for (int u = 0; u < nLeft; u++) {
        memset(visitedRight, 0, sizeof(visitedRight));
        if (tryKuhn(u)) {
            matching = matching + 1;
            printf("Worker %d matched to job %d (total %d)\n",
                   u, matchLeft[u], matching);
        } else {
            printf("Worker %d could NOT be matched\n", u);
        }
    }

    printf("\nMaximum matching = %d\n", matching);
    printf("Final pairs:\n");
    for (int u = 0; u < nLeft; u++) {
        if (matchLeft[u] != -1) {
            printf("  worker %d -> job %d\n", u, matchLeft[u]);
        }
    }

    return 0;
}
