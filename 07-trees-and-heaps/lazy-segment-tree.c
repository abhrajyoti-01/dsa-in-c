/*
Lazy Segment Tree - Range Additions and Range Sums (Advanced)
------------------------------------------------------------
PROBLEM DEFINITION:
Interleave additions to inclusive ranges with sum queries, both in
O(log n). Extend the point-update segment tree with deferred updates.

REAL-LIFE EXAMPLE:
Increase every salary in a department's index interval, then query
payroll totals for arbitrary intervals without visiting every employee.

DIAGRAM (array [1,2,3,4], add 5 to all):
              sum=10 -> 30, lazy=5
             /                    \
          sum=3                  sum=7   (children not changed YET)
  A partial query pushes +5 down: left sum=13, right sum=17.
  Invariant: a node's sum ALREADY includes its own lazy addition;
  lazy records only what its children have not yet received.

PSEUDO CODE:
  Apply(node,l,r,delta): sum[node] += delta*(r-l+1); lazy[node] += delta
  Push: Apply the pending addition to both children; clear parent lazy
  Add/Query:
      No overlap: return (0 for query)
      Full overlap: Apply addition OR return stored sum
      Partial overlap: Push, recurse into children
      After an update: recompute parent sum from children

SAMPLE INPUT & DRY RUN:
  4
  1 2 3 4
  4
  2 0 3
  1 1 3 5
  2 0 3
  2 1 2
  Operation 2 queries; operation 1 adds delta.
  Initial sum = 10. Update gives [1,7,8,9].
  New full sum = 25; sum(1,2) = 15.

SPECIAL CASES:
  - Negative/zero additions, overlapping updates, and n=1 are valid.
  - All indices are zero-based and ranges are inclusive.
  - n and operation count <= 1000, values/deltas within +/-10^9.
    long long safely holds all sums under these limits.
  - Updates are ADDITIONS, not assignments: lazy tags accumulate.

COMPLEXITY:
  O(n) build and storage; O(log n) per update/query; O(log n) stack.

TIPS:
  Never multiply an addition by the whole array length: use the NODE'S
  segment length. Push before descending, not before returning full cover.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_OPERATIONS 1000
#define VALUE_LIMIT 1000000000LL

long long arr[MAX_N];
long long tree[4 * MAX_N];
long long lazy[4 * MAX_N];

void buildTree(int node, int l, int r) {
    if (l == r) {
        tree[node] = arr[l];
        return;
    }
    int mid = l + (r - l) / 2;
    buildTree(node * 2, l, mid);
    buildTree(node * 2 + 1, mid + 1, r);
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

void applyAddition(int node, int l, int r, long long delta) {
    tree[node] += delta * (r - l + 1);
    lazy[node] += delta;
}

void push(int node, int l, int r) {
    if (lazy[node] != 0 && l != r) {
        int mid = l + (r - l) / 2;
        applyAddition(node * 2, l, mid, lazy[node]);
        applyAddition(node * 2 + 1, mid + 1, r, lazy[node]);
        lazy[node] = 0;
    }
}

void rangeAdd(int node, int l, int r, int ql, int qr, long long delta) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        applyAddition(node, l, r, delta);
        return;
    }
    push(node, l, r);
    int mid = l + (r - l) / 2;
    rangeAdd(node * 2, l, mid, ql, qr, delta);
    rangeAdd(node * 2 + 1, mid + 1, r, ql, qr, delta);
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

long long querySum(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return tree[node];
    push(node, l, r);
    int mid = l + (r - l) / 2;
    return querySum(node * 2, l, mid, ql, qr) +
           querySum(node * 2 + 1, mid + 1, r, ql, qr);
}

int main(void) {
    int n;
    int q;
    printf("Lazy Segment Tree - Range Add / Range Sum\n\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9): ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &arr[i]) != 1 ||
            arr[i] < -VALUE_LIMIT || arr[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    buildTree(1, 0, n - 1);
    printf("How many operations (0-%d)? ", MAX_OPERATIONS);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_OPERATIONS) {
        printf("Invalid operation count\n");
        return 1;
    }
    printf("1 l r delta = range add; 2 l r = range sum (zero-based)\n");
    for (int i = 0; i < q; i++) {
        int type;
        int l;
        int r;
        if (scanf("%d %d %d", &type, &l, &r) != 3 ||
            (type != 1 && type != 2) || l < 0 || r >= n || l > r) {
            printf("Invalid operation or range\n");
            return 1;
        }
        if (type == 1) {
            long long delta;
            if (scanf("%lld", &delta) != 1 ||
                delta < -VALUE_LIMIT || delta > VALUE_LIMIT) {
                printf("Invalid addition\n");
                return 1;
            }
            rangeAdd(1, 0, n - 1, l, r, delta);
            printf("Added %lld to [%d..%d]\n", delta, l, r);
        } else {
            printf("sum(%d, %d) = %lld\n", l, r,
                   querySum(1, 0, n - 1, l, r));
        }
    }
    return 0;
}
