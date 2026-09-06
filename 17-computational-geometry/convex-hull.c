/*
Convex Hull - Andrew's Monotone Chain
-------------------------------------
What this program solves:
Finds the CONVEX HULL of points in the plane: the smallest convex
polygon containing all points - the "rubber band snapped around
pushpins" shape.

Real life example:
Stretching a rubber band around pins; GPS geofencing and image
outlining compute the same minimal boundary - the convex hull.

DIAGRAM (monotone chain: lower hull then upper hull):
  sort by x; walk left->right keeping only LEFT turns (lower hull)
  walk right->left for the upper hull; concatenate.
  1 3 5 -> hull vertices in order, no clockwise turns left inside.

BEGINNER EXPLANATION (the simplest way to think):
Put pins on a board, stretch a rubber band around all of them and
release - the band settles on the hull. Algorithmically: sort
points left-to-right; walk left to right keeping a "lower fence"
that only makes LEFT turns, then walk right-to-left making left
turns again for the upper side. Both walks together trace the
boundary. The cross-product test (which way do we turn?) is the
only geometry needed.

SUPER SIMPLE EXAMPLE:
Points (0,0), (1,1), (2,0), (1,-1):
hull = (0,0) -> (2,0) -> (1,1)? No wait - (1,1) sticks UP: hull is
triangle (0,0), (2,0), (1,1). Point (1,1)? It IS a vertex. Point
INSIDE like (1,0.2) would be excluded - it sits inside the triangle.
Hull answers: which points are on the OUTSIDE boundary.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- All points collinear: hull is a LINE segment (2 extreme points),
  not a polygon - some definitions keep collinear boundary points,
  this one keeps only the strict corners.
- Duplicate points: harmless; the sort handles them, cross-product
  test rejects zero-area turns.
- 1 or 2 points: hull is the point/segment itself (degenerate).
- n < 3 can't form a polygon - the program reports the degenerate
  case rather than producing garbage.

DRY RUN (lower hull, points sorted by x):
(0,0) (1,1) (2,0) (3,3)
push (0,0), push (1,1)
candidate (2,0): cross((0,0)->(1,1), (1,1)->(2,0)) = 1*(-1)-1*2
= -3 < 0 -> right turn -> POP (1,1), push (2,0). Hull: (0,0),(2,0)
candidate (3,3): cross((0,0)->(2,0), (2,0)->(3,3)) = 2*3 - 0*1
= 6 >= 0 -> left turn, push. Lower hull: (0,0),(2,0),(3,3)
Upper walk mirrors this -> final hull excludes interior points.

TIPS:
- O(n log n) - the sort dominates; the walks are O(n) total since
  each point is pushed/popped once (amortized, like a stack).
- Cross product of (B-A) x (C-A): positive = C is LEFT of line AB,
  negative = right, zero = collinear. Memorize: (Bx-Ax)(Cy-Ay) -
  (By-Ay)(Cx-Ax).
- Applications: collision detection, geographic fence (geofencing),
  computing the diameter (farthest pair = hull vertices only).
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_P 500

typedef struct {
    long long x;
    long long y;
} Point;

Point pts[MAX_P];
Point hull[2 * MAX_P];

int comparePoints(const void* a, const void* b) {
    Point* p = (Point*)a;
    Point* q = (Point*)b;
    if (p->x != q->x) {
        return (p->x < q->x) ? -1 : 1;
    }
    return (p->y < q->y) ? -1 : (p->y > q->y) ? 1 : 0;
}

/*
 * cross - >0 left turn, <0 right turn, 0 collinear.
 */
long long cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int main() {
    int n;

    printf("Convex Hull (Andrew's Monotone Chain)\n\n");

    printf("How many points (1-%d)? ", MAX_P);
    scanf("%d", &n);

    if (n < 1 || n > MAX_P) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter x y for each point (integers):\n");
    for (int i = 0; i < n; i++) {
        printf("  Point %d: ", i + 1);
        scanf("%lld %lld", &pts[i].x, &pts[i].y);
    }

    qsort(pts, (size_t)n, sizeof(Point), comparePoints);

    if (n == 1) {
        printf("\nHull (degenerate single point): (%lld, %lld)\n",
               pts[0].x, pts[0].y);
        return 0;
    }
    if (n == 2) {
        printf("\nHull (degenerate segment): (%lld, %lld) - (%lld, %lld)\n",
               pts[0].x, pts[0].y, pts[1].x, pts[1].y);
        return 0;
    }

    /* STEP: lower hull - keep left turns */
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) {
            k = k - 1;
        }
        hull[k] = pts[i];
        k = k + 1;
    }

    /* STEP: upper hull - walk the sorted points backwards */
    int lower = k + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (k >= lower && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) {
            k = k - 1;
        }
        hull[k] = pts[i];
        k = k + 1;
    }

    int hullSize = k - 1;

    printf("\nConvex hull vertices (%d):\n", hullSize);
    for (int i = 0; i < hullSize; i++) {
        printf("  (%lld, %lld)\n", hull[i].x, hull[i].y);
    }

    /* STEP: identify which input points are hull vertices */
    printf("\nInterior/excluded points: ");
    int excluded = 0;
    for (int i = 0; i < n; i++) {
        int onHull = 0;
        for (int j = 0; j < hullSize; j++) {
            if (hull[i].x == pts[j].x && hull[i].y == pts[j].y) {
                onHull = 1;
                break;
            }
        }
        int isVertex = 0;
        for (int j = 0; j < hullSize; j++) {
            if (hull[j].x == pts[i].x && hull[j].y == pts[i].y) {
                isVertex = 1;
                break;
            }
        }
        if (!isVertex) {
            printf("  (%lld, %lld)\n", pts[i].x, pts[i].y);
            excluded = excluded + 1;
        }
        (void)onHull;
    }
    if (excluded == 0) {
        printf("  (none - all points are hull vertices)\n");
    }

    return 0;
}
