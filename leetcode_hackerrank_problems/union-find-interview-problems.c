/*
Union-Find Interview Problems - Provinces, Redundant Edge, Islands II,
Accounts Merge (Intermediate -> Advanced)
------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 547  - Number of Provinces (friend circles)
  LeetCode 684  - Redundant Connection
  LeetCode 721  - Accounts Merge
  LeetCode 959  - Regions Cut By Slashes (grid + union-find)
  HackerRank "Merging Communities" (union sizes on demand)
  HackerRank "Kundu and Tree" style connectivity counting

PROBLEM DEFINITION:
Model friendships, extra cables and shared e-mail accounts as unions of
sets. Union-Find answers "same group?" and "merge these groups" in
nearly constant amortised time, which beats building the graph and
running BFS for the streaming and repeated-query versions.

REAL-LIFE EXAMPLE:
Customer identity resolution: two accounts belong to the same person
when they share an e-mail, phone or card. Each shared attribute is an
edge; the connected groups are the real people - accounts merge.

DIAGRAM (provinces, n = 6, friendships 1-2, 2-3, 4-5):
  sets after unions: {1,2,3}  {4,5}  {6}       -> 3 provinces
  redundant cable on 1-2,2-3,1-3: the third edge closes the cycle
  accounts merge: (a, x@m) (b, y@m) (a, z@m)
     x@m joins a and b; z@m joins a (already with b) -> one person

PSEUDO CODE:
  Find(x) with path compression; Union(a, b) by rank/size
  Provinces(n, edges): union every edge; count i with Find(i) == i
  RedundantEdge(edges): for each edge, if Find(u) == Find(v) it is the
      extra one; else union. Process edges in input order.
  AccountsMerge: index every e-mail; union the account's first e-mail
      with the rest; the groups are the merged accounts
  MergingCommunities: keep size[] per root; merging adds the sizes;
      a query answers size[Find(x)]

SAMPLE INPUT & DRY RUN:
  6                       <- part 1: provinces: how many nodes
  3                       <- how many friendships
  1 2
  2 3
  4 5                     <- provinces: 3 friendships over 6 nodes
  3                       <- part 2: redundant connection: how many edges
  1 2
  2 3
  1 3                     <- the 3rd edge closes a cycle -> redundant
  3                       <- part 3: accounts merge: how many accounts
  2 x@m y@m               <- account 1 holds x@m and y@m
  2 x@m z@m               <- account 2 shares x@m -> merged with account 1
  2 f@g f@g               <- account 3 is a separate person
  2                       <- part 4: merging communities: how many ops
  1 1 2                   <- merge the groups of 1 and 2
  2 1                     <- query the group size of node 1
  Provinces = 3 ({1,2,3}, {4,5}, {6}); the redundant edge is (1,3)
  because 1 and 3 are already connected; a 3-node component has size 3.
  Accounts merge into 2 people: person 1 owns accounts 1 and 2 with
  e-mails x@m y@m z@m; person 2 owns account 3 with f@g.
  After merging 1 with 2 the group of node 1 has size 2.

SPECIAL CASES:
  - Self-loop (u == v) is immediately redundant.
  - Repeated identical edges: the second copy is redundant.
  - Node IDs are 1..n (human friendly); the code maps internally.
  - m < n - 1 is fine: the graph is simply a forest with more parts.
  - The redundant edge is unique only when the input is a tree plus one
    edge; otherwise the LAST closing edge in input order is reported.
  - n <= 1000, edges <= 2000, e-mail strings <= 40 characters.

COMPLEXITY:
  O((n + m) * alpha(n)) time with path compression, O(n) space; the
  inverse Ackermann factor alpha(n) <= 4 for every practical input.

TIPS:
  Union-Find is the right tool when edges arrive ONLINE or when many
  "are these connected?" queries follow. When you need actual paths
  (shortest route, ancestors) use BFS/DFS instead. Always implement
  path compression AND union by size - one without the other loses the
  amortised bound.
*/

#include <stdio.h>
#include <string.h>

#define MAX_NODES 1000
#define MAX_EDGES 2000
#define MAX_EMAILS 100
#define MAX_EMAIL_LEN 40

int parent[MAX_NODES + 1];
int sizeOf[MAX_NODES + 1];

int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];      /* path halving */
        x = parent[x];
    }
    return x;
}

void makeSet(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        sizeOf[i] = 1;
    }
}

int unionSets(int a, int b) {
    int rootA = findRoot(a);
    int rootB = findRoot(b);
    if (rootA == rootB) return 0;
    if (sizeOf[rootA] < sizeOf[rootB]) {
        int temp = rootA;
        rootA = rootB;
        rootB = temp;
    }
    parent[rootB] = rootA;
    sizeOf[rootA] += sizeOf[rootB];
    return 1;
}

int componentsOf(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (findRoot(i) == i) count++;
    }
    return count;
}

int main(void) {
    int n;
    int edges;

    printf("Union-Find Interview Problems\n\n");
    printf("== Provinces: how many nodes (1-%d)? ", MAX_NODES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    printf("How many friendships (0-%d)? ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid edge count\n");
        return 1;
    }
    makeSet(n);
    printf("Enter %d edges as 'u v' (1..%d):\n", edges, n);
    for (int i = 0; i < edges; i++) {
        int u;
        int v;
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 ||
            v > n) {
            printf("Invalid edge\n");
            return 1;
        }
        int merged = unionSets(u, v);
        printf("   edge %d-%d: %s\n", u, v, merged ? "merged" : "already same set");
    }
    printf("Provinces (connected groups) = %d\n", componentsOf(n));
    printf("Largest province size = ");
    int largest = 0;
    for (int i = 1; i <= n; i++) {
        if (findRoot(i) == i && sizeOf[i] > largest) largest = sizeOf[i];
    }
    printf("%d\n", largest);

    printf("\n== Redundant connection: how many edges (1-%d)? ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 1 || edges > MAX_EDGES) {
        printf("Invalid edge count\n");
        return 1;
    }
    makeSet(n);
    int redundantU = 0;
    int redundantV = 0;
    printf("Enter %d edges as 'u v' (1..%d), processed in order:\n", edges,
           n);
    for (int i = 0; i < edges; i++) {
        int u;
        int v;
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 ||
            v > n) {
            printf("Invalid edge\n");
            return 1;
        }
        if (u == v) {
            redundantU = u;
            redundantV = v;
            printf("   edge %d-%d is a self-loop -> redundant\n", u, v);
            continue;
        }
        if (!unionSets(u, v)) {
            redundantU = u;
            redundantV = v;
            printf("   edge %d-%d closes a cycle -> redundant\n", u, v);
        }
    }
    if (redundantU != 0) {
        printf("Last redundant edge = %d-%d\n", redundantU, redundantV);
    } else {
        printf("No redundant edge: the input is a forest\n");
    }
    printf("Components after processing = %d\n", componentsOf(n));

    printf("\n== Accounts merge: how many accounts (1-%d)? ", MAX_EMAILS);
    int accounts;
    if (scanf("%d", &accounts) != 1 || accounts < 1 || accounts > MAX_EMAILS) {
        printf("Invalid account count\n");
        return 1;
    }
    char emails[MAX_EMAILS * 4][MAX_EMAIL_LEN + 1];
    int emailCount = 0;
    int accountOwner[MAX_EMAILS * 4];
    makeSet(MAX_NODES);
    printf("For each account enter 'count' then that many e-mails (max 4 each,\n");
    printf("lowercase e-mails of length 3..%d):\n", MAX_EMAIL_LEN);
    for (int account = 1; account <= accounts; account++) {
        int count;
        printf("Account %d e-mail count: ", account);
        if (scanf("%d", &count) != 1 || count < 1 || count > 4) {
            printf("Invalid e-mail count\n");
            return 1;
        }
        int firstIndex = -1;
        for (int j = 0; j < count; j++) {
            char email[MAX_EMAIL_LEN + 1];
            if (scanf("%40s", email) != 1 || strlen(email) < 3) {
                printf("Invalid e-mail\n");
                return 1;
            }
            int index = -1;
            for (int t = 0; t < emailCount; t++) {
                if (strcmp(emails[t], email) == 0) {
                    index = t;
                    break;
                }
            }
            if (index == -1) {
                if (emailCount >= MAX_EMAILS * 4) {
                    printf("Too many distinct e-mails\n");
                    return 1;
                }
                strcpy(emails[emailCount], email);
                index = emailCount;
                accountOwner[emailCount] = account;
                emailCount++;
            } else if (accountOwner[index] != account) {
                unionSets(accountOwner[index], account);
            }
            if (firstIndex == -1) {
                firstIndex = index;
            } else {
                unionSets(accountOwner[firstIndex], accountOwner[index]);
            }
        }
    }
    int mergesPrinted = 0;
    for (int account = 1; account <= accounts; account++) {
        if (findRoot(account) != account) continue;
        mergesPrinted++;
        printf("  person %d owns accounts:", mergesPrinted);
        for (int other = 1; other <= accounts; other++) {
            if (findRoot(other) == account) printf(" %d", other);
        }
        printf(" with e-mails:");
        for (int t = 0; t < emailCount; t++) {
            if (findRoot(accountOwner[t]) == account) {
                printf(" %s", emails[t]);
            }
        }
        printf("\n");
    }
    printf("Distinct people after merging = %d\n", mergesPrinted);

    printf("\n== Merging communities: node id 1..%d, ", n);
    printf("how many merge/query operations (0-%d)? ", MAX_EDGES);
    int operations;
    if (scanf("%d", &operations) != 1 || operations < 0 ||
        operations > MAX_EDGES) {
        printf("Invalid operation count\n");
        return 1;
    }
    makeSet(n);
    printf("Enter operations: '1 u v' to merge, '2 u' to query the group size:\n");
    for (int i = 0; i < operations; i++) {
        int type;
        if (scanf("%d", &type) != 1 || (type != 1 && type != 2)) {
            printf("Invalid operation\n");
            return 1;
        }
        if (type == 1) {
            int u;
            int v;
            if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 ||
                v > n) {
                printf("Invalid merge\n");
                return 1;
            }
            unionSets(u, v);
            printf("   merged %d and %d\n", u, v);
        } else {
            int u;
            if (scanf("%d", &u) != 1 || u < 1 || u > n) {
                printf("Invalid query\n");
                return 1;
            }
            printf("   community size of %d = %d\n", u, sizeOf[findRoot(u)]);
        }
    }
    printf("\nDone: every part is Find/Union plus one counter.\n");
    return 0;
}
