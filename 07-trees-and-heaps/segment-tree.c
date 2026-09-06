/*
 PROBLEM DEFINITION:
 --------------------
 A Segment Tree is a binary tree used to store information about intervals
 (segments/ranges). It allows answering range queries and point updates
 in O(log N) time. Each node in the segment tree represents a segment
 (range) of the array, and the tree stores aggregated information like
 sum, minimum, maximum, or other functions over those ranges.

 REAL-LIFE EXAMPLE - Weather Monitoring Station:
 ------------------------------------------------
 Imagine a weather station monitoring temperatures across 8 cities.
 You need to quickly find:
 - Maximum temperature in cities 3 to 7
 - Minimum temperature in cities 1 to 5
 - Sum of temperatures in cities 2 to 6

 Instead of scanning all cities for each query (O(N)), a segment tree
 organizes the data hierarchically so queries take O(log N) time.
 Each node stores the aggregate (max/min/sum) for its range.

 DIAGRAM (array [1, 3, 5, 7, 9, 11, 13, 15], sum tree):
   tree node          range it covers
   root [64]          [0..7]  (everything)
    /        \
 [16]        [48]      [0..3]    [4..7]
  /  \        /  \
 [4] [12]  [20]  [28]     [0..1][2..3][4..5][6..7]
 / \  / \   / \   / \
 1  3  5  7 9  11 13  15  (leaves = array elements)

 Range sum(2,5) = [12] + [20] = 32 - only 2 nodes, not 4 scans!
 Point update arr[4]=20 walks root->right->(4,5)->leaf, then
 recomputes exactly those 4 nodes.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 The process of organizing and querying ranges can be compared to
 organizing weather data in a hierarchical monitoring system:

 1. BUILDING THE TREE:
    - Start with the entire array at the root.
    - Recursively divide the array into two halves.
    - Build left child for left half, right child for right half.
    - Each node stores the aggregate (sum/min/max) of its segment.
    Example for array [1, 3, 5, 7, 9, 11]:
    - Root (0-5): stores sum of all = 36
    - Left child (0-2): stores sum of [1,3,5] = 9
    - Right child (3-5): stores sum of [7,9,11] = 27
    - Continue dividing until single elements.

 2. RANGE QUERY:
    - Start at the root (entire range).
    - If query range completely covers node range, use node's value.
    - If no overlap, return identity (0 for sum, INF for min).
    - If partial overlap, recurse on both children and combine.
    Example: Query sum(1,4) on [1,3,5,7,9,11]:
    - Root (0-5) partial overlaps -> query children
    - Left (0-2) partial overlaps (1-2) -> query children
    - Leaf 1=3, Leaf 2=5 -> sum=8
    - Right (3-5) contains 3-4 -> 7+9=16
    - Total: 8+16=24

 3. POINT UPDATE:
    - Navigate to the leaf representing the position.
    - Update the leaf value.
    - Recompute aggregates on the path back to root.
    Example: Update position 2 from 5 to 10:
    - Leaf 2 becomes 10 (was 5)
    - Node (1-2): 3+10=13 (was 9)
    - Node (0-2): 1+13=14 (was 10)
    - Root: 14+27=41 (was 36)

 PSEUDO CODE:
 ------------
 START
   Function Build(node, start, end):
     If start == end:
         tree[node] = arr[start]
     Else:
         mid = (start + end) / 2
         Build(node*2, start, mid)
         Build(node*2+1, mid+1, end)
         tree[node] = tree[node*2] + tree[node*2+1]

   Function Query(node, start, end, queryStart, queryEnd):
     If query range completely outside: Return 0
     If query range completely inside: Return tree[node]
     Partial overlap:
         leftAns = Query(node*2, start, mid, queryStart, queryEnd)
         rightAns = Query(node*2+1, mid+1, end, queryStart, queryEnd)
         Return leftAns + rightAns

   Function Update(node, start, end, index, newValue):
     If start == end:
         tree[node] = newValue
     Else:
         mid = (start + end) / 2
         If index <= mid:
             Update(node*2, start, mid, index, newValue)
         Else:
             Update(node*2+1, mid+1, end, index, newValue)
         tree[node] = tree[node*2] + tree[node*2+1]
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Array: [1, 3, 5, 7, 9, 11, 13, 15] (8 elements)

 Building Segment Tree (Sum queries):

 Level 0: Root (0-7): Sum = 1+3+5+7+9+11+13+15 = 64
         /                        \
 Level 1: Left (0-3): 16          Right (4-7): 48
         /    \                    /    \
 Level 2: (0-1):4 (2-3):12    (4-5):20  (6-7):28
         /  \   /  \           /  \      /  \
 Level 3: 1  3  5   7         9   11    13  15

 Query sum(2, 5):
 - Root (0-7): partial overlap -> recurse
 - Left (0-3): partial overlap (2-3) -> recurse
   - (0-1): no overlap -> return 0
   - (2-3): fully inside -> return 12
 - Right (4-7): partial overlap (4-5) -> recurse
   - (4-5): fully inside -> return 20
   - (6-7): no overlap -> return 0
 - Answer: 12 + 20 = 32

 Update position 4 to value 20 (was 9):
 - Navigate: Root -> Right(4-7) -> (4-5) -> Leaf 4
 - Leaf 4 = 20
 - (4-5) = 20+11 = 31 (was 20)
 - Right(4-7) = 31+13+15 = 59 (was 48)
 - Root = 16+59 = 75 (was 64)

 TIME COMPLEXITY:
 - Build Tree:   O(N)
 - Range Query:  O(log N)
 - Point Update: O(log N)

 SPACE COMPLEXITY: O(N) (at most 4N array size)

 COMMON USE CASES:
 - Competitive programming range queries
 - Database indexing (range scans)
 - Computational geometry problems
 - Game development (spatial queries)
 - Financial analysis (stock price ranges)
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

/*
 * Global segment tree array.
 * tree[1] is the root. For node i:
 * - Left child:  2*i
 * - Right child: 2*i + 1
 * - Parent:      i / 2
 */
int tree[MAX_SIZE];
int originalArray[MAX_SIZE];
int arraySize;

/*
 * buildTree - Recursively builds the segment tree.
 *
 * node:   current tree node index (1-based)
 * start:  start index of the segment in the array
 * end:    end index of the segment in the array
 *
 * Analogy: Dividing cities into groups for weather monitoring.
 * Each manager (node) handles a specific group of cities (range).
 */
void buildTree(int node, int start, int end) {
    if (start == end) {
        tree[node] = originalArray[start];
        return;
    }

    int mid = (start + end) / 2;

    buildTree(2 * node, start, mid);
    buildTree(2 * node + 1, mid + 1, end);

    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

/*
 * querySum - Answers a range sum query in O(log N).
 *
 * node:       current tree node index
 * start,end:  segment covered by this node
 * l,r:        query range we are interested in
 *
 * Three cases:
 * 1. No overlap:      return 0 (identity for sum)
 * 2. Complete overlap: return node's stored value
 * 3. Partial overlap:  recurse both children and combine
 */
int querySum(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return 0;
    }

    if (l <= start && end <= r) {
        return tree[node];
    }

    int mid = (start + end) / 2;

    int leftSum = querySum(2 * node, start, mid, l, r);
    int rightSum = querySum(2 * node + 1, mid + 1, end, l, r);

    return leftSum + rightSum;
}

/*
 * updateValue - Updates a single element and refreshes ancestors.
 *
 * node:      current tree node index
 * start,end: segment covered by this node
 * index:     position in the original array to update
 * newValue:  the new value
 *
 * Analogy: A city updates its temperature; the manager must recalculate
 * the group's aggregate all the way up to the chief.
 */
void updateValue(int node, int start, int end, int index, int newValue) {
    if (start == end) {
        originalArray[index] = newValue;
        tree[node] = newValue;
        return;
    }

    int mid = (start + end) / 2;

    if (index <= mid) {
        updateValue(2 * node, start, mid, index, newValue);
    } else {
        updateValue(2 * node + 1, mid + 1, end, index, newValue);
    }

    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

/*
 * queryMin - Answers a range minimum query in O(log N).
 * Demonstrates how the same tree structure supports different aggregations.
 */
int queryMin(int node, int start, int end, int l, int r, int* minTree) {
    if (r < start || end < l) {
        return 2147483647;
    }

    if (l <= start && end <= r) {
        return minTree[node];
    }

    int mid = (start + end) / 2;

    int leftMin = queryMin(2 * node, start, mid, l, r, minTree);
    int rightMin = queryMin(2 * node + 1, mid + 1, end, l, r, minTree);

    return (leftMin < rightMin) ? leftMin : rightMin;
}

/*
 * buildMinTree - Builds a segment tree storing minimum values.
 */
void buildMinTree(int node, int start, int end, int* minTree) {
    if (start == end) {
        minTree[node] = originalArray[start];
        return;
    }

    int mid = (start + end) / 2;

    buildMinTree(2 * node, start, mid, minTree);
    buildMinTree(2 * node + 1, mid + 1, end, minTree);

    minTree[node] = (minTree[2 * node] < minTree[2 * node + 1])
                    ? minTree[2 * node] : minTree[2 * node + 1];
}

/*
 * printArray - Utility to display the array contents.
 */
void printArray(void) {
    printf("[");
    for (int i = 0; i < arraySize; i++) {
        printf("%d", originalArray[i]);
        if (i < arraySize - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int n;
    int choice;

    printf("Segment Tree - Range Sum and Min Queries\n\n");

    printf("How many elements (1-%d)? ", MAX_SIZE);
    scanf("%d", &n);

    if (n < 1 || n > MAX_SIZE) {
        printf("Invalid count\n");
        return 1;
    }

    arraySize = n;

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < arraySize; i++) {
        scanf("%d", &originalArray[i]);
    }

    printf("\nArray: ");
    printArray();

    /* STEP: build the sum segment tree */
    buildTree(1, 0, arraySize - 1);

    /* STEP: build the min segment tree */
    int* minTree = (int*)malloc(sizeof(int) * (4 * MAX_SIZE));
    if (minTree == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    buildMinTree(1, 0, arraySize - 1, minTree);

    while (1) {
        printf("\nSegment Tree Operations\n");
        printf("1. Range sum query sum(l, r)\n");
        printf("2. Range min query min(l, r)\n");
        printf("3. Point update arr[i] = value\n");
        printf("4. Display array\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int l;
            int r;
            printf("Enter l and r (0..%d): ", arraySize - 1);
            scanf("%d %d", &l, &r);
            if (l < 0 || r >= arraySize || l > r) {
                printf("Invalid range\n");
            } else {
                printf("sum(%d, %d) = %d\n", l, r,
                       querySum(1, 0, arraySize - 1, l, r));
            }
        } else if (choice == 2) {
            int l;
            int r;
            printf("Enter l and r (0..%d): ", arraySize - 1);
            scanf("%d %d", &l, &r);
            if (l < 0 || r >= arraySize || l > r) {
                printf("Invalid range\n");
            } else {
                printf("min(%d, %d) = %d\n", l, r,
                       queryMin(1, 0, arraySize - 1, l, r, minTree));
            }
        } else if (choice == 3) {
            int idx;
            int value;
            printf("Enter index and new value: ");
            scanf("%d %d", &idx, &value);
            if (idx < 0 || idx >= arraySize) {
                printf("Invalid index\n");
            } else {
                updateValue(1, 0, arraySize - 1, idx, value);
                printf("Updated array: ");
                printArray();
            }
        } else if (choice == 4) {
            printf("Array: ");
            printArray();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    free(minTree);
    return 0;
}
