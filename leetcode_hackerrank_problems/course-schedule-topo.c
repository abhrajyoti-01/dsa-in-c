/*
Course Schedule - Prerequisites, Ordering, and Cycle Reporting (Intermediate -> Advanced)
----------------------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 207 - Course Schedule (is a valid order possible?)
  LeetCode 210 - Course Schedule II (produce one valid order)
  LeetCode 269 - Alien Dictionary (topological order from comparisons)
  LeetCode 802 - Find Eventual Safe States (reverse-graph reasoning)
  HackerRank "Topological Sort" (direct equivalent of part 2)

PROBLEM DEFINITION:
Prerequisites form a directed graph: edge prerequisite -> course. A
valid study plan exists exactly when the graph has no directed cycle,
and any topological order is a valid plan. Kahn's algorithm peels
courses with in-degree 0 one by one.

REAL-LIFE EXAMPLE:
A university curriculum: some modules require others first. The
registrar needs a semester plan (a topological order) and must warn
students when requirements are circular (a cycle).

DIAGRAM (4 courses, prerequisites 1->0, 2->0, 3->1, 3->2):
  in-degree: c0=2, c1=1, c2=1, c3=0
  peel 3 (in-degree 0) -> c1 = 0, c2 = 0
  peel 1 -> c0 = 1; peel 2 -> c0 = 0; peel 0
  order: 3 1 2 0        (equally valid: 3 2 1 0)
  add edge 0 -> 3 and the graph becomes cyclic: no plan exists.

PSEUDO CODE:
  Build adjacency lists and in-degree counts
  Queue every course with in-degree 0
  While the queue is not empty:
      pop u, append u to the order
      for each v in adjacency[u]: decrement in-degree[v];
          if it reaches 0, push v
  If the order contains all courses: acyclic (print it)
  Otherwise the remaining courses sit on a cycle: report which ones

SAMPLE INPUT & DRY RUN:
  4
  4
  1 0
  2 0
  3 1
  3 2
  Acyclic: order 3 1 2 0 (all 4 courses). The leftover set after Kahn
  is empty, so a plan exists. Adding 0 -> 3 leaves courses {0,3} on a
  cycle -> "no valid plan".

SPECIAL CASES:
  - No prerequisites: every course is available at once, and the order
    is the course numbering itself.
  - Multiple valid orders: any topological order is accepted; this
    program proves validity by checking every edge's direction in the
    produced order (a self-check worth copying).
  - Self-prerequisite (u == u): an immediate cycle.
  - Duplicate edges inflate in-degree; they are counted once here by
    rejecting duplicates at input time.
  - Cyclic input: the leftover courses (in-degree never reaching 0) are
    exactly the cycle membership - printing them helps debugging.
  - courses <= 1000, prerequisite pairs <= 2000.

COMPLEXITY:
  O(courses + prerequisites) time and space: each edge is relaxed once.
  Cycle DETECTION could also use DFS colours, but Kahn also yields the
  order for free - prefer it for scheduling problems.

TIPS:
  Whenever a problem says "ordering with dependencies", think
  topological sort. When it says "can it be finished", you only need
  the cycle test: the number of peeled courses equals the count of all
  courses exactly when the graph is acyclic.
*/

#include <stdio.h>

#define MAX_COURSES 1000
#define MAX_PAIRS 2000

int head[MAX_COURSES];
int to[MAX_PAIRS];
int nextEdge[MAX_PAIRS];
int edgeCount;
int inDegree[MAX_COURSES];
int order[MAX_COURSES];
int peeled[MAX_COURSES];

void addEdge(int from, int target) {
    to[edgeCount] = target;
    nextEdge[edgeCount] = head[from];
    head[from] = edgeCount++;
    inDegree[target]++;
}

int hasEdge(int from, int target) {
    for (int e = head[from]; e != -1; e = nextEdge[e]) {
        if (to[e] == target) return 1;
    }
    return 0;
}

int main(void) {
    int courses;
    int pairs;

    printf("Course Schedule - Topological Ordering\n\n");
    printf("How many courses (1-%d)? ", MAX_COURSES);
    if (scanf("%d", &courses) != 1 || courses < 1 ||
        courses > MAX_COURSES) {
        printf("Invalid course count\n");
        return 1;
    }
    printf("How many prerequisites (0-%d)? ", MAX_PAIRS);
    if (scanf("%d", &pairs) != 1 || pairs < 0 || pairs > MAX_PAIRS) {
        printf("Invalid pair count\n");
        return 1;
    }
    for (int i = 0; i < courses; i++) {
        head[i] = -1;
        inDegree[i] = 0;
        peeled[i] = 0;
    }
    printf("Enter %d pairs as 'prerequisite course' (0-based ids):\n", pairs);
    for (int i = 0; i < pairs; i++) {
        int prerequisite;
        int course;
        if (scanf("%d %d", &prerequisite, &course) != 2 ||
            prerequisite < 0 || prerequisite >= courses || course < 0 ||
            course >= courses) {
            printf("Invalid prerequisite pair\n");
            return 1;
        }
        if (prerequisite == course) {
            printf("Self-prerequisite %d -> %d creates a cycle\n",
                   prerequisite, course);
        } else if (hasEdge(prerequisite, course)) {
            printf("Duplicate pair %d -> %d ignored\n", prerequisite, course);
        } else {
            addEdge(prerequisite, course);
        }
    }

    int front = 0;
    int rear = 0;
    int ordered = 0;
    for (int i = 0; i < courses; i++) {
        if (inDegree[i] == 0) order[rear++] = i;
    }
    while (front < rear) {
        int course = order[front++];
        ordered++;
        for (int e = head[course]; e != -1; e = nextEdge[e]) {
            if (--inDegree[to[e]] == 0) order[rear++] = to[e];
        }
    }

    if (ordered == courses) {
        int position[MAX_COURSES];
        printf("\nA valid study plan exists.\nOrder:");
        for (int i = 0; i < ordered; i++) {
            printf(" %d", order[i]);
            position[order[i]] = i;
        }
        printf("\n");
        int valid = 1;
        for (int u = 0; u < courses; u++) {
            for (int e = head[u]; e != -1; e = nextEdge[e]) {
                if (position[u] >= position[to[e]]) valid = 0;
            }
        }
        printf("Self-check: every prerequisite appears earlier = %s\n",
               valid ? "yes" : "NO (bug!)");
    } else {
        printf("\nNo valid plan: %d course(s) remain unpeeled ",
               courses - ordered);
        printf("and lie on a cycle.\nBlocked courses:");
        for (int i = 0; i < courses; i++) {
            if (peeled[i] == 0 && inDegree[i] > 0) printf(" %d", i);
        }
        printf("\nThe graph is cyclic (LeetCode 207 answers false).\n");
    }
    return 0;
}
