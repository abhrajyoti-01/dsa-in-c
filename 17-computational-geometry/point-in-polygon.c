/*
Point in Polygon - Exact Ray Casting (Intermediate -> Advanced)
--------------------------------------------------------------
PROBLEM DEFINITION:
Classify points as INSIDE, OUTSIDE, or BOUNDARY of a simple polygon.
Works for concave as well as convex polygons, in either vertex order.

REAL-LIFE EXAMPLE:
A map checks whether a location lies in a delivery zone. Treating
boundary points explicitly avoids ambiguous decisions along streets.

DIAGRAM:
  (0,4) +----------+ (4,4)
        |   P---->|--------> ray to the right crosses once -> INSIDE
        |         |
  (0,0) +----------+ (4,0)
  A ray from outside crosses an even number of sides (possibly zero).

PSEUDO CODE:
  For every polygon side a->b:
      If point is collinear and inside its bounding box: BOUNDARY
      If (a.y > p.y) differs from (b.y > p.y):
          cross = (b-a) x (p-a)
          If cross sign matches (b.y-a.y): toggle inside
  Return INSIDE if toggled odd times, otherwise OUTSIDE

SAMPLE INPUT & DRY RUN:
  4
  0 0
  4 0
  4 4
  0 4
  3
  2 2
  5 2
  4 2
  (2,2): only the right edge crosses its rightward ray -> INSIDE.
  (5,2): no rightward crossings -> OUTSIDE.
  (4,2): collinear and on the right edge -> BOUNDARY.

SPECIAL CASES:
  - Horizontal edges do not count as crossings but can contain the point.
  - Strict '>' on y uses half-open crossings: vertices are not counted twice.
  - List vertices in boundary order without repeating the first at the end.
  - Self-intersections, duplicate vertices, backtracking edges, and zero
    area are rejected. Consecutive collinear sides in one direction are OK.
  - n is 3..200, queries 0..1000, integer coordinates within +/-10^6.
    Cross products and signed area sums fit safely in long long.

COMPLEXITY:
  Polygon validation O(n^2), each query O(n), polygon storage O(n).

TIPS:
  Multiplying cross products can overflow: compare their SIGNS instead.
  No floating-point division or epsilon is needed with bounded integers.
*/

#include <stdio.h>

#define MAX_N 200
#define MAX_QUERIES 1000
#define COORD_LIMIT 1000000LL

typedef struct {
    long long x;
    long long y;
} Point;

long long cross(Point a, Point b, Point p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}

int onSegment(Point a, Point b, Point p) {
    return cross(a, b, p) == 0 &&
           p.x >= (a.x < b.x ? a.x : b.x) && p.x <= (a.x > b.x ? a.x : b.x) &&
           p.y >= (a.y < b.y ? a.y : b.y) && p.y <= (a.y > b.y ? a.y : b.y);
}

int oppositeSigns(long long a, long long b) {
    return (a < 0 && b > 0) || (a > 0 && b < 0);
}

int intersects(Point a, Point b, Point c, Point d) {
    long long c1 = cross(a, b, c);
    long long c2 = cross(a, b, d);
    long long c3 = cross(c, d, a);
    long long c4 = cross(c, d, b);
    return (oppositeSigns(c1, c2) && oppositeSigns(c3, c4)) ||
           onSegment(a, b, c) || onSegment(a, b, d) ||
           onSegment(c, d, a) || onSegment(c, d, b);
}

int isSimplePolygon(const Point polygon[], int n) {
    long long twiceArea = 0;
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];
        Point c = polygon[(i + 2) % n];
        twiceArea += a.x * b.y - a.y * b.x;
        /* Adjacent sides may share only their common endpoint. */
        if (onSegment(a, b, c) || onSegment(b, c, a)) return 0;
        for (int j = i + 1; j < n; j++) {
            if (a.x == polygon[j].x && a.y == polygon[j].y) return 0;
            if (j == i + 1 || (i == 0 && j == n - 1)) continue;
            if (intersects(a, b, polygon[j], polygon[(j + 1) % n])) return 0;
        }
    }
    return twiceArea != 0;
}

/* Returns 0 outside, 1 inside, 2 on the boundary. */
int classifyPoint(const Point polygon[], int n, Point p) {
    int inside = 0;
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];
        if (onSegment(a, b, p)) return 2;
        if ((a.y > p.y) != (b.y > p.y)) {
            long long side = cross(a, b, p);
            if ((b.y > a.y && side > 0) || (b.y < a.y && side < 0)) {
                inside = !inside;
            }
        }
    }
    return inside;
}

int readPoint(Point* point) {
    return scanf("%lld %lld", &point->x, &point->y) == 2 &&
           point->x >= -COORD_LIMIT && point->x <= COORD_LIMIT &&
           point->y >= -COORD_LIMIT && point->y <= COORD_LIMIT;
}

int main(void) {
    Point polygon[MAX_N];
    int n;
    int q;
    const char* labels[] = {"OUTSIDE", "INSIDE", "BOUNDARY"};
    printf("Point in Polygon - Ray Casting\n\n");
    printf("How many polygon vertices (3-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 3 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter vertices in boundary order as 'x y' (within +/-10^6):\n");
    for (int i = 0; i < n; i++) {
        if (!readPoint(&polygon[i])) {
            printf("Invalid coordinate\n");
            return 1;
        }
    }
    if (!isSimplePolygon(polygon, n)) {
        printf("Invalid polygon: require a simple boundary with nonzero area\n");
        return 1;
    }
    printf("How many query points (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        Point p;
        printf("Point %d: x y: ", i + 1);
        if (!readPoint(&p)) {
            printf("Invalid coordinate\n");
            return 1;
        }
        printf("Point (%lld, %lld): %s\n", p.x, p.y, labels[classifyPoint(polygon, n, p)]);
    }
    return 0;
}
