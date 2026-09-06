/*
Closest Pair of Points (Divide and Conquer)
-------------------------------------------
What this program solves:
Finds the two closest points among n points in the plane in
O(n log n) - versus the naive O(n^2) all-pairs scan - by splitting
on the median x and checking only a narrow middle strip.

Real life example:
Air traffic control: which two aircraft are dangerously close?
GPS fleet analysis, molecular docking, and VLSI design all boil
down to closest-pair queries on thousands/millions of points.

DIAGRAM (split by x, conquer, check the middle strip):
  points | strip of width 2*delta around the dividing line
  left half -> dL    right half -> dR    delta = min(dL, dR)
  only pairs INSIDE the strip, sorted by y, need re-checking
  (a bounded number of neighbours each) -> O(n log n).

How the process works:
1. Sort points by x once.
2. Recurse: split at mid; solve left half and right half.
   d = min(left answer, right answer).
3. Strip step: points within d of the dividing line, sorted by y;
   each point needs comparison with at most the next 7 neighbors
   (geometry proves it), so the merge is O(n).
4. Recurrence: T(n) = 2T(n/2) + O(n) -> O(n log n).

Pseudo code:
START
CLOSEST(P sorted by x):
    IF n <= 3: brute force
    mid = n/2
    dL = CLOSEST(left); dR = CLOSEST(right); d = MIN
    strip = points with |x - x[mid]| < d, sorted by y
    FOR each strip point: compare to next 7 strip points
RETURN overall minimum
END

Sample input and dry run:
Points: (2,3) (12,30) (40,50) (5,1) (12,10) (3,4)
Brute force would try 15 pairs; divide & conquer finds
(2,3)-(3,1) with distance sqrt(5) after 2 splits.

Main logic to understand:
"Split, solve halves, patch the middle": the strip check handles
pairs straddling the split that neither half saw. The classic
recurrence T(n)=2T(n/2)+O(n) yields n log n - a 1000x speedup
over brute force at n = 100k.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

int compareX(const void* a, const void* b) {
    Point* p = (Point*)a;
    Point* q = (Point*)b;
    if (p->x < q->x) {
        return -1;
    }
    if (p->x > q->x) {
        return 1;
    }
    return 0;
}

/*
 * distSq - Squared distance (avoids sqrt in comparisons).
 */
double distSq(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

/*
 * bruteForce - O(n^2) scan for tiny inputs.
 */
double bruteForce(Point pts[], int n, Point* bestA, Point* bestB) {
    double best = 1e30;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = distSq(pts[i], pts[j]);
            if (d < best) {
                best = d;
                *bestA = pts[i];
                *bestB = pts[j];
            }
        }
    }
    return best;
}

double closestPair(Point px[], int n, Point* bestA, Point* bestB) {
    if (n <= 3) {
        return bruteForce(px, n, bestA, bestB);
    }

    int mid = n / 2;
    double midX = px[mid].x;

    Point pairL1;
    Point pairL2;
    Point pairR1;
    Point pairR2;

    double dL = closestPair(px, mid, &pairL1, &pairL2);
    double dR = closestPair(px + mid, n - mid, &pairR1, &pairR2);

    double d = dL;
    Point localA = pairL1;
    Point localB = pairL2;
    if (dR < dL) {
        d = dR;
        localA = pairR1;
        localB = pairR2;
    }

    /* STEP: strip of points within d of the dividing line */
    Point strip[2000];
    int stripCount = 0;
    for (int i = 0; i < n; i++) {
        double dx = px[i].x - midX;
        if (dx < 0) {
            dx = -dx;
        }
        if (dx * dx < d) {
            strip[stripCount] = px[i];
            stripCount = stripCount + 1;
        }
    }

    for (int i = 0; i < stripCount; i++) {
        for (int j = i + 1; j < stripCount && j <= i + 7; j++) {
            double dy = strip[i].y - strip[j].y;
            if (dy * dy < d) {
                double dNew = distSq(strip[i], strip[j]);
                if (dNew < d) {
                    d = dNew;
                    localA = strip[i];
                    localB = strip[j];
                }
            }
        }
    }

    *bestA = localA;
    *bestB = localB;
    return d;
}

int main() {
    Point pts[2000];
    int n;

    printf("Closest Pair of Points (Divide & Conquer)\n\n");

    printf("How many points (2-2000)? ");
    scanf("%d", &n);

    if (n < 2 || n > 2000) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter x y for each point:\n");
    for (int i = 0; i < n; i++) {
        printf("  Point %d: ", i + 1);
        scanf("%lf %lf", &pts[i].x, &pts[i].y);
    }

    qsort(pts, (size_t)n, sizeof(Point), compareX);

    Point a;
    Point b;
    double best = closestPair(pts, n, &a, &b);

    printf("\nClosest pair: (%.2f, %.2f) and (%.2f, %.2f)\n",
           a.x, a.y, b.x, b.y);
    printf("Distance: %.4f (squared: %.4f)\n",
           sqrt(best), best);

    /* STEP: brute-force verification */
    Point ba;
    Point bb;
    double verify = bruteForce(pts, n, &ba, &bb);
    printf("Brute-force verification matches: %s\n",
           (verify == best) ? "YES" : "NO");

    return 0;
}
