/*
Union-Find (Disjoint Set Union) with Union by Rank and Path Compression
-----------------------------------------------------------------------
What this program solves:
Tracks elements split into disjoint sets: FIND(x) returns the set
representative, UNION(a, b) merges two sets. With the two
optimizations, both run in near-constant amortized time.

Real life example:
Friend circles in a social network: when two people become friends,
their entire friend groups merge. "Are A and B in the same circle?"
is a FIND query. Kruskal's MST algorithm uses this to avoid cycles.

DIAGRAM (union by rank + path compression):
  before: 1->2->3 (find 1 walks two links)
  find(1): re-point 1 and 2 DIRECTLY to 3:
           1-->3<--2            (flattened tree)
  union: attach the SHORTER tree under the TALLER root.

How the process works:
1. parent[i] = i initially - everyone is their own representative.
2. rank[i] = 0 approximates tree height.
3. FIND: follow parent links to the root, then re-point every node
   on the path directly to the root (PATH COMPRESSION).
4. UNION: attach the shorter tree under the taller root
   (UNION BY RANK), so trees stay shallow.

Pseudo code:
START
FIND(x): WHILE parent[x] != x: x = parent[x]  (then compress path)
UNION(a, b): ra = FIND(a), rb = FIND(b)
    IF ra == rb: already same set
    IF rank[ra] < rank[rb]: parent[ra] = rb
    ELSE IF rank[ra] > rank[rb]: parent[rb] = ra
    ELSE: parent[rb] = ra; rank[ra]++
END

Sample input and dry run:
union(1,2) union(3,4) union(2,3)
find(1) -> root of {1,2,3,4}; find(1) == find(4) -> same set

Main logic to understand:
Without optimizations FIND is O(n); with union by rank + path
compression the amortized cost is alpha(n) < 5 for any practical n
(inverse Ackermann). This powers Kruskal, connected components, and
cycle detection.
*/

#include <stdio.h>

#define MAX_ELEMENTS 100

int parent[MAX_ELEMENTS];
int rankArr[MAX_ELEMENTS];

/*
 * makeSet - Every element starts as its own set.
 */
void makeSet(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        rankArr[i] = 0;
    }
}

/*
 * findRoot - Returns set representative; compresses the path.
 */
int findRoot(int x) {
    if (parent[x] != x) {
        parent[x] = findRoot(parent[x]);
    }
    return parent[x];
}

/*
 * unionSets - Merges two sets using union by rank.
 */
void unionSets(int a, int b) {
    int ra = findRoot(a);
    int rb = findRoot(b);

    if (ra == rb) {
        printf("%d and %d are ALREADY in the same set\n", a, b);
        return;
    }

    if (rankArr[ra] < rankArr[rb]) {
        parent[ra] = rb;
        printf("Root %d now points to root %d\n", ra, rb);
    } else if (rankArr[ra] > rankArr[rb]) {
        parent[rb] = ra;
        printf("Root %d now points to root %d\n", rb, ra);
    } else {
        parent[rb] = ra;
        rankArr[ra] = rankArr[ra] + 1;
        printf("Root %d points to root %d, rank of %d raised to %d\n",
               rb, ra, ra, rankArr[ra]);
    }
}

/*
 * displaySets - Groups elements by their representative.
 */
void displaySets(int n) {
    printf("\nSets:\n");
    for (int i = 1; i <= n; i++) {
        int root = findRoot(i);
        if (root == i || findRoot(i) == i) {
            printf("  Set { ");
            for (int j = 1; j <= n; j++) {
                if (findRoot(j) == i) {
                    printf("%d ", j);
                }
            }
            printf("} (representative %d)\n", i);
        }
    }
}

int main() {
    int n;
    int choice;
    int a;
    int b;

    printf("Union-Find (Disjoint Set Union)\n\n");

    printf("How many elements (1-%d, numbered 1..n)? ", MAX_ELEMENTS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ELEMENTS) {
        printf("Invalid count\n");
        return 1;
    }

    makeSet(n);
    printf("%d singleton sets created\n", n);

    while (1) {
        printf("\n1. Union(a, b)\n");
        printf("2. Find(a)\n");
        printf("3. Same set? (a, b)\n");
        printf("4. Display all sets\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter a and b: ");
            scanf("%d %d", &a, &b);
            if (a >= 1 && a <= n && b >= 1 && b <= n) {
                unionSets(a, b);
            } else {
                printf("Invalid elements\n");
            }
        } else if (choice == 2) {
            printf("Enter element: ");
            scanf("%d", &a);
            if (a >= 1 && a <= n) {
                printf("find(%d) = %d\n", a, findRoot(a));
            } else {
                printf("Invalid element\n");
            }
        } else if (choice == 3) {
            printf("Enter a and b: ");
            scanf("%d %d", &a, &b);
            if (a >= 1 && a <= n && b >= 1 && b <= n) {
                printf("%d and %d are in the same set: %s\n",
                       a, b, findRoot(a) == findRoot(b) ? "YES" : "NO");
            } else {
                printf("Invalid elements\n");
            }
        } else if (choice == 4) {
            displaySets(n);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
