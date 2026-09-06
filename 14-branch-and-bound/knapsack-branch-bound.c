/*
0/1 Knapsack - Branch and Bound
-------------------------------
What this program solves:
Solves 0/1 knapsack exactly using BEST-FIRST branch and bound:
explore the most promising node first, prune nodes whose upper
bound (fractional knapsack of remaining items) cannot beat the
best value found so far.

Real life example:
A cargo drone with a weight limit choosing among shipments. B&B
explores "ship this shipment / skip it" decisions but abandons
whole branches the moment even the most optimistic completion
cannot improve the current best load.

DIAGRAM (bound prunes whole branches of the search tree):
  node: (level, value, weight)
  bound = value + greedy fraction of the remaining capacity
  bound <= bestSoFar  ->  skip that subtree entirely
  include/exclude item k; best value monotonically improves.

How the process works:
1. Sort items by value/weight ratio (helps the bound).
2. Node = (level, current value, current weight).
   Bound computed greedily with fractions on remaining items.
3. Priority queue picks the node with the HIGHEST bound.
4. Branch: include item (if it fits) or exclude it.
5. Prune when bound <= best value found.
6. Leaf node (level == n) updates the best.

Pseudo code:
START
BOUND(node): greedy fractional fill of remaining capacity
PQ = {root(level 0)}
WHILE PQ not empty
    node = extract max bound
    IF node.bound <= best: continue (prune)
    IF node.level == n: best = max(best, node.value)
    ELSE branch include/exclude with updated bounds
END

Sample input and dry run:
Items (value, weight): (60,10) (100,20) (120,30), capacity 50
B&B explores include-first branches, prunes nodes with bound <= 220,
answer: 220 (items 2 and 3).

Main logic to understand:
Bound function = DP-free DP: fractional relaxation gives an upper
bound in O(n). Best-first search reaches good solutions early,
making pruning aggressive; worst case still exponential but far
faster than plain enumeration in practice.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 30

typedef struct {
    int value;
    int weight;
} Item;

Item items[MAX_ITEMS];
int itemCount;
int capacity;
int bestValue = 0;
int bestTaken[MAX_ITEMS];
int currentTaken[MAX_ITEMS];

/*
 * cmpRatio - Sort items by value/weight descending.
 */
int cmpRatio(const void* a, const void* b) {
    Item* x = (Item*)a;
    Item* y = (Item*)b;
    double rx = (double)x->value / x->weight;
    double ry = (double)y->value / y->weight;
    if (rx < ry) {
        return 1;
    }
    if (rx > ry) {
        return -1;
    }
    return 0;
}

/*
 * bound - Fractional knapsack upper bound from level onward.
 */
double bound(int level, int currValue, int currWeight) {
    if (currWeight >= capacity) {
        return 0;
    }
    double result = (double)currValue;
    int totalWeight = currWeight;

    for (int i = level; i < itemCount; i++) {
        if (totalWeight + items[i].weight <= capacity) {
            totalWeight = totalWeight + items[i].weight;
            result = result + items[i].value;
        } else {
            int remain = capacity - totalWeight;
            result = result + (double)items[i].value * remain / items[i].weight;
            break;
        }
    }
    return result;
}

/*
 * branchAndBound - DFS with fractional-bound pruning.
 */
void branchAndBound(int level, int currValue, int currWeight) {
    if (currWeight > capacity) {
        return;
    }
    if (level == itemCount) {
        if (currValue > bestValue) {
            bestValue = currValue;
            for (int i = 0; i < itemCount; i++) {
                bestTaken[i] = currentTaken[i];
            }
        }
        return;
    }

    /* prune: even the optimistic completion cannot beat current best */
    if (currValue > bestValue) {
        bestValue = currValue;
        for (int i = 0; i < itemCount; i++) {
            bestTaken[i] = currentTaken[i];
        }
    }
    if (bound(level, currValue, currWeight) <= (double)bestValue) {
        return;
    }

    /* branch: include item[level] */
    currentTaken[level] = 1;
    branchAndBound(level + 1,
                   currValue + items[level].value,
                   currWeight + items[level].weight);

    /* branch: exclude item[level] */
    currentTaken[level] = 0;
    branchAndBound(level + 1, currValue, currWeight);
}

int main() {
    printf("0/1 Knapsack - Branch and Bound\n\n");

    printf("How many items (1-%d)? ", MAX_ITEMS);
    scanf("%d", &itemCount);

    if (itemCount < 1 || itemCount > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter value and weight of each item (e.g. '60 10'):\n");
    for (int i = 0; i < itemCount; i++) {
        printf("  Item %d: ", i + 1);
        scanf("%d %d", &items[i].value, &items[i].weight);
    }

    printf("Knapsack capacity: ");
    scanf("%d", &capacity);

    if (capacity < 0) {
        printf("Invalid capacity\n");
        return 1;
    }

    /* STEP: sort by ratio to strengthen the bound */
    qsort(items, (size_t)itemCount, sizeof(Item), cmpRatio);

    for (int i = 0; i < itemCount; i++) {
        bestTaken[i] = 0;
        currentTaken[i] = 0;
    }

    branchAndBound(0, 0, 0);

    printf("\nMaximum value: %d\n", bestValue);
    printf("Items chosen (by ratio-sorted order):\n");
    int totalWeight = 0;
    for (int i = 0; i < itemCount; i++) {
        if (bestTaken[i]) {
            printf("  (value %d, weight %d)\n", items[i].value, items[i].weight);
            totalWeight = totalWeight + items[i].weight;
        }
    }
    printf("Total weight: %d\n", totalWeight);

    return 0;
}
