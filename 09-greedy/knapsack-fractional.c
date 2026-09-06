/*
Fractional Knapsack
-------------------
What this program solves:
Suppose a shopkeeper has many valuable materials such as rice, sugar, dry fruits,
or gold powder, and a customer can carry only a limited weight. If the material
can be divided, the best choice is not always the item with the highest total
profit, but the item that gives the highest profit per unit weight. This program
finds that maximum possible profit.

Real life example:
Imagine a delivery bag that can carry only 50 kg. You have:
item A -> profit 60, weight 10
item B -> profit 100, weight 20
item C -> profit 120, weight 30
Profit per kg becomes 6, 5, and 4.
So a smart person first fills the bag with A, then B, and then takes only the
remaining part of C. Because the items are divisible, taking a fraction is allowed.

DIAGRAM (capacity 50; items (v,w): A(60,10) B(100,20) C(120,30)):
  value/weight: A=6 B=5 C=4 -> take in that order
  A full (10kg), B full (20kg) -> 20kg left, C is 30kg
  take 20/30 of C -> 80 value
  total = 60 + 100 + 80 = 240

How the process works:
1. For every item, calculate profit / weight.
2. Sort all items from highest ratio to lowest ratio.
3. Start filling the knapsack from the best ratio item.
4. If an item fully fits, take it completely.
5. If it does not fully fit, take only the fraction that can still fit.
6. Stop when the bag becomes full.

Pseudo code:
START
Read n, capacity, profit, and weight of items
For each item
    Compute ratio = profit / weight
Sort items in descending order of ratio
Set remaining capacity = total capacity
For each item in sorted order
    If full item fits
        Take the whole item
        Reduce remaining capacity
    Else
        Take only the needed fraction
        Stop
Print fractions taken and total profit
END

Sample input and dry run:
Capacity = 50
Items: (profit, weight) = (60,10), (100,20), (120,30)
Ratios become 6, 5, and 4.
Take first item fully, remaining capacity = 40.
Take second item fully, remaining capacity = 20.
Take 20/30 part of the third item, so total profit = 60 + 100 + 80 = 240.

Why this greedy logic works:
Since fractions are allowed, taking the currently best profit-per-unit item
always improves the answer most quickly.
*/
#include <stdio.h>

#define MAX 100

struct Item {
    int id;
    float profit;
    float weight;
    float ratio;
};

int main() {
    struct Item items[MAX], temp;
    float fraction[MAX] = {0};
    int n, i, j;
    float capacity, totalProfit = 0, remaining;

    printf("Fractional Knapsack Algorithm\n");
    printf("Enter the number of items: ");
    scanf("%d", &n);

    printf("Enter the capacity of the knapsack: ");
    scanf("%f", &capacity);

    for (i = 0; i < n; i++) {
        items[i].id = i + 1;
        printf("Enter profit for item %d: ", i + 1);
        scanf("%f", &items[i].profit);
        printf("Enter weight for item %d: ", i + 1);
        scanf("%f", &items[i].weight);
        items[i].ratio = items[i].profit / items[i].weight;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (items[j].ratio < items[j + 1].ratio) {
                temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    remaining = capacity;

    for (i = 0; i < n; i++) {
        if (items[i].weight <= remaining) {
            fraction[items[i].id - 1] = 1.0;
            remaining -= items[i].weight;
            totalProfit += items[i].profit;
        } else {
            fraction[items[i].id - 1] = remaining / items[i].weight;
            totalProfit += items[i].profit * fraction[items[i].id - 1];
            break;
        }
    }

    printf("\nFractions of items taken:\n");
    for (i = 0; i < n; i++) {
        printf("Item %d: %.2f\n", i + 1, fraction[i]);
    }

    printf("Total maximum profit: %.2f\n", totalProfit);
    return 0;
}

