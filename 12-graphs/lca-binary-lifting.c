/*
Lowest Common Ancestor - Binary Lifting (Advanced)
-------------------------------------------------
PROBLEM DEFINITION:
For a rooted, undirected tree, answer many lowest-common-ancestor and
edge-distance queries. Unlike a BST, node IDs have no ordering property.

REAL-LIFE EXAMPLE:
Find the nearest common manager of two employees in a reporting tree,
or the shared directory ancestor of two files in a filesystem tree.

DIAGRAM (root 0):
       0
      / \
     1   2       LCA(3,4) = 1, distance = 2
    / \          LCA(3,2) = 0, distance = 3
   3   4
  up[v][k] stores the ancestor 2^k edges above v.

PSEUDO CODE:
  BFS from root to compute depth and immediate parents
  up[v][k] = up[up[v][k-1]][k-1]
  LCA(u,v):
      Lift the deeper vertex by the bits of the depth difference
      If equal: return u
      For k from largest to 0:
          If up[u][k] != up[v][k]: lift both
      Return their immediate parent
  distance(u,v) = depth[u] + depth[v] - 2*depth[LCA(u,v)]

SAMPLE INPUT & DRY RUN:
  5
  0 1
  0 2
  1 3
  1 4
  0
  2
  3 4
  3 2
  Query (3,4): equal depths, parents both 1 -> LCA 1, distance 2.
  Query (3,2): lift 3 to 1; parents of 1 and 2 are 0 -> distance 3.

SPECIAL CASES:
  - A vertex is its own ancestor: LCA(u,u)=u, distance 0.
  - n=1 works with no edges. Root's ancestor entries point to itself.
  - Read exactly n-1 edges; require connectivity to validate a tree.
    Disconnected/cyclic input and self-loops are rejected.
  - Vertex IDs are 0..n-1, n <= 1000, query count <= 1000.

COMPLEXITY:
  O(n log n) preprocessing/storage; O(log n) per query.

TIPS:
  BFS avoids a deep recursive DFS on a chain. The root affects LCA,
  but does not affect the number of edges between two vertices.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_LOG 10
#define MAX_QUERIES 1000

int head[MAX_N];
int to[2 * MAX_N];
int nextEdge[2 * MAX_N];
int edgeCount;
int depth[MAX_N];
int up[MAX_N][MAX_LOG];

void addEdge(int u, int v) {
    to[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount++;
}

int buildAncestors(int n, int root) {
    int queue[MAX_N];
    int front = 0;
    int rear = 0;
    for (int i = 0; i < n; i++) depth[i] = -1;
    depth[root] = 0;
    up[root][0] = root;
    queue[rear++] = root;
    while (front < rear) {
        int u = queue[front++];
        for (int e = head[u]; e != -1; e = nextEdge[e]) {
            int v = to[e];
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                up[v][0] = u;
                queue[rear++] = v;
            }
        }
    }
    /* A connected undirected graph with n-1 edges is a tree. */
    if (rear != n) return 0;
    for (int k = 1; k < MAX_LOG; k++) {
        for (int v = 0; v < n; v++) {
            up[v][k] = up[up[v][k - 1]][k - 1];
        }
    }
    return 1;
}

int lowestCommonAncestor(int u, int v) {
    if (depth[u] < depth[v]) {
        int temp = u;
        u = v;
        v = temp;
    }
    int difference = depth[u] - depth[v];
    for (int k = 0; k < MAX_LOG; k++) {
        if (difference & (1 << k)) u = up[u][k];
    }
    if (u == v) return u;
    for (int k = MAX_LOG - 1; k >= 0; k--) {
        if (up[u][k] != up[v][k]) {
            u = up[u][k];
            v = up[v][k];
        }
    }
    return up[u][0];
}

int main(void) {
    int n;
    int root;
    int q;
    printf("Lowest Common Ancestor - Binary Lifting\n\n");
    printf("How many vertices (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < n; i++) head[i] = -1;
    printf("Enter %d undirected edges as 'u v' (zero-based):\n", n - 1);
    for (int i = 0; i < n - 1; i++) {
        int u;
        int v;
        if (scanf("%d %d", &u, &v) != 2 ||
            u < 0 || u >= n || v < 0 || v >= n || u == v) {
            printf("Invalid edge\n");
            return 1;
        }
        addEdge(u, v);
        addEdge(v, u);
    }
    printf("Root vertex: ");
    if (scanf("%d", &root) != 1 || root < 0 || root >= n) {
        printf("Invalid root\n");
        return 1;
    }
    if (!buildAncestors(n, root)) {
        printf("Invalid tree: graph must be connected with n-1 edges\n");
        return 1;
    }
    printf("How many queries (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        int u;
        int v;
        printf("Query %d: u v: ", i + 1);
        if (scanf("%d %d", &u, &v) != 2 ||
            u < 0 || u >= n || v < 0 || v >= n) {
            printf("Invalid query vertices\n");
            return 1;
        }
        int ancestor = lowestCommonAncestor(u, v);
        printf("LCA(%d, %d) = %d; distance = %d\n", u, v, ancestor,
               depth[u] + depth[v] - 2 * depth[ancestor]);
    }
    return 0;
}
