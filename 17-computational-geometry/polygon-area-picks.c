/*
Polygon Area and Pick's Theorem - Shoelace Formula on Lattice Points (Intermediate)
----------------------------------------------------------------------------------
PROBLEM DEFINITION:
Given a simple polygon whose vertices have integer coordinates, compute:
  - its exact area with the shoelace formula (no square roots),
  - its orientation (counter-clockwise or clockwise),
  - B, the number of lattice points on its boundary (via gcd),
  - I, the number of lattice points strictly inside, using Pick's
    theorem  A = I + B/2 - 1  ->  I = A - B/2 + 1.
For small polygons the program also counts the points one by one to
confirm Pick's theorem.

REAL-LIFE EXAMPLE:
A surveyor computes the area of a plot from the corner coordinates;
a farmer planting trees on a 1-metre grid wants to know how many trees
fit strictly inside the fence and how many fence posts stand on grid
points. GIS software and game maps compute polygon areas the same way.

DIAGRAM (sample polygon, a house shape):
   y
   5 .  .  *  .  .          * = vertex, o = boundary point,
   4 .  o  #  o  .          # = interior point
   3 *  #  #  #  *
   2 o  #  #  #  o          vertices (0,0) (4,0) (4,3) (2,5) (0,3)
   1 o  #  #  #  o
   0 *  o  o  o  *
     0  1  2  3  4  x

PSEUDO CODE:
  twiceArea = sum over edges (xi*y(i+1) - x(i+1)*yi)   (signed)
  orientation = CCW if twiceArea > 0 else CW
  B = sum over edges gcd(|dx|, |dy|)
  I = (|twiceArea| - B + 2) / 2                        (Pick)
  Simple-polygon check: no two non-adjacent edges touch, and
  adjacent edges do not fold back onto each other.

SAMPLE INPUT & DRY RUN:
  5
  0 0
  4 0
  4 3
  2 5
  0 3
  shoelace terms: 0, 4*3-4*0 = 12, 4*5-2*3 = 14, 2*3-0*5 = 6, 0
  twiceArea = 32 -> area 16, counter-clockwise
  B = gcd(4,0) + gcd(0,3) + gcd(2,2) + gcd(2,2) + gcd(0,3)
    = 4 + 3 + 2 + 2 + 3 = 14
  I = (32 - 14 + 2) / 2 = 10     (count the #s in the diagram: 10)

SPECIAL CASES:
  - Clockwise input gives a negative signed area; the absolute value
    is the area.
  - Collinear consecutive vertices are allowed (they just split an edge).
  - Self-intersecting polygons ("bow ties") and zero-area polygons are
    rejected: the shoelace formula and Pick's theorem need a simple
    polygon.
  - 3 <= n <= 1000 vertices in order, |coordinates| <= 10^6. The
    point-by-point check runs only when it costs <= 2 * 10^7 steps.

COMPLEXITY:
  Area, B and I: O(n). Simplicity check: O(n^2). Brute-force count:
  O(width * height * n).

TIPS:
  The shoelace formula is the sum of signed triangle areas from the
  origin - the same cross product used in convex-hull. Pick's theorem
  only works for lattice polygons. Compare point-in-polygon (the
  boundary/inside test used for the check) and line-segment-intersection.
*/

#include <stdio.h>

#define MAX_N 1000
#define COORDINATE_LIMIT 1000000LL
#define CHECK_BUDGET 20000000LL

long long xs[MAX_N];
long long ys[MAX_N];
int n;

long long cross(long long ax, long long ay, long long bx, long long by,
                long long cx, long long cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

long long absolute(long long v) {
    return v < 0 ? -v : v;
}

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int sign(long long v) {
    return (v > 0) - (v < 0);
}

/* Is (px, py) on segment a-b (endpoints included)? Assumes collinear. */
int withinBox(long long ax, long long ay, long long bx, long long by,
              long long px, long long py) {
    return px >= (ax < bx ? ax : bx) && px <= (ax > bx ? ax : bx) &&
           py >= (ay < by ? ay : by) && py <= (ay > by ? ay : by);
}

int segmentsTouch(int i, int j) {
    int i2 = (i + 1) % n;
    int j2 = (j + 1) % n;
    int d1 = sign(cross(xs[i], ys[i], xs[i2], ys[i2], xs[j], ys[j]));
    int d2 = sign(cross(xs[i], ys[i], xs[i2], ys[i2], xs[j2], ys[j2]));
    int d3 = sign(cross(xs[j], ys[j], xs[j2], ys[j2], xs[i], ys[i]));
    int d4 = sign(cross(xs[j], ys[j], xs[j2], ys[j2], xs[i2], ys[i2]));
    if (d1 * d2 < 0 && d3 * d4 < 0) return 1;
    if (d1 == 0 && withinBox(xs[i], ys[i], xs[i2], ys[i2], xs[j], ys[j])) return 1;
    if (d2 == 0 && withinBox(xs[i], ys[i], xs[i2], ys[i2], xs[j2], ys[j2])) return 1;
    if (d3 == 0 && withinBox(xs[j], ys[j], xs[j2], ys[j2], xs[i], ys[i])) return 1;
    if (d4 == 0 && withinBox(xs[j], ys[j], xs[j2], ys[j2], xs[i2], ys[i2])) return 1;
    return 0;
}

int isSimple(void) {
    for (int i = 0; i < n; i++) {
        int a = i;
        int b = (i + 1) % n;
        int c = (i + 2) % n;
        /* Adjacent edges a-b, b-c must not fold back along each other. */
        if (cross(xs[a], ys[a], xs[b], ys[b], xs[c], ys[c]) == 0 &&
            (xs[b] - xs[a]) * (xs[c] - xs[b]) + (ys[b] - ys[a]) * (ys[c] - ys[b]) <= 0) {
            return 0;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (j == i + 1 || (i == 0 && j == n - 1)) continue;
            if (segmentsTouch(i, j)) return 0;
        }
    }
    return 1;
}

/* 0 = outside, 1 = inside, 2 = on the boundary. */
int classify(long long px, long long py) {
    int inside = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (cross(xs[i], ys[i], xs[j], ys[j], px, py) == 0 &&
            withinBox(xs[i], ys[i], xs[j], ys[j], px, py)) {
            return 2;
        }
        if ((ys[i] > py) != (ys[j] > py)) {
            /* Edge crosses the horizontal line y = py: is the crossing right of px? */
            long long side = cross(xs[i], ys[i], xs[j], ys[j], px, py);
            if ((side > 0) == (ys[j] > ys[i])) inside = !inside;
        }
    }
    return inside;
}

int main(void) {
    printf("Polygon Area and Pick's Theorem\n\n");
    printf("Number of vertices (3-%d): ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 3 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        printf("Vertex %d: x y: ", i + 1);
        if (scanf("%lld %lld", &xs[i], &ys[i]) != 2 ||
            absolute(xs[i]) > COORDINATE_LIMIT ||
            absolute(ys[i]) > COORDINATE_LIMIT) {
            printf("Invalid vertex\n");
            return 1;
        }
    }

    long long twiceArea = 0;
    long long boundary = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        twiceArea += xs[i] * ys[j] - xs[j] * ys[i];
        boundary += gcd(absolute(xs[j] - xs[i]), absolute(ys[j] - ys[i]));
    }
    if (twiceArea == 0 || !isSimple()) {
        printf("\nNot a simple polygon with positive area - "
               "the formulas do not apply.\n");
        return 1;
    }

    long long area2 = absolute(twiceArea);
    long long interior = (area2 - boundary + 2) / 2;
    printf("\nSigned twice-area: %lld -> %s\n", twiceArea,
           twiceArea > 0 ? "counter-clockwise" : "clockwise");
    printf("Area: %lld%s\n", area2 / 2, area2 % 2 ? ".5" : "");
    printf("Boundary lattice points B: %lld\n", boundary);
    printf("Interior lattice points I (Pick): %lld\n", interior);

    long long minX = xs[0], maxX = xs[0], minY = ys[0], maxY = ys[0];
    for (int i = 1; i < n; i++) {
        if (xs[i] < minX) minX = xs[i];
        if (xs[i] > maxX) maxX = xs[i];
        if (ys[i] < minY) minY = ys[i];
        if (ys[i] > maxY) maxY = ys[i];
    }
    long long width = maxX - minX + 1;
    long long height = maxY - minY + 1;
    if (width <= CHECK_BUDGET / height / n) {
        long long countInside = 0;
        long long countBoundary = 0;
        for (long long x = minX; x <= maxX; x++) {
            for (long long y = minY; y <= maxY; y++) {
                int where = classify(x, y);
                if (where == 1) countInside++;
                else if (where == 2) countBoundary++;
            }
        }
        printf("Point-by-point count: B = %lld, I = %lld -> %s\n",
               countBoundary, countInside,
               countBoundary == boundary && countInside == interior
                   ? "Pick's theorem confirmed" : "MISMATCH");
    }
    return 0;
}
