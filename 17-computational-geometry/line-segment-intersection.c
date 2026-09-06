/*
Line Segment Intersection - Orientation (CCW) Test
---------------------------------------------------
What this program solves:
Determines whether two line segments AB and CD intersect - using
only the CCW (counter-clockwise) orientation test. The foundation
of computational geometry: used by the convex hull, polygon
intersection, and graphics clipping.

Real life example:
Map software detects road crossings; game engines and CAD tools
test segment overlap every frame using orientation tests.

DIAGRAM (orientation via cross product (CCW test)):
  cross(b-a, c-a) > 0 : c is LEFT of a->b (counter-clockwise)
  segments AB and CD intersect when A,B are on OPPOSITE sides of
  CD and C,D on opposite sides of AB.

BEGINNER EXPLANATION (the simplest way to think):
Two segments cross if and only if:
- Each segment STRADDLES the other: A and B are on opposite sides
  of the line through C-D, AND C and D are on opposite sides of
  the line through A-B.
"How do I know which side a point is on?" - the cross product!
It answers 'left', 'right', or 'exactly on the line'.

SUPER SIMPLE EXAMPLE:
Segment (0,0)-(4,4) and (0,4)-(4,0): they cross at (2,2).
A(0,0), B(4,4) vs line CD (from (0,4) to (4,0)): A is below-right,
B is above -> straddle. C,D vs line AB: same test straddles too
-> they INTERSECT.
Segment (0,0)-(1,1) and (2,2)-(3,3): both collinear and disjoint ->
orientation is 0 (on-line) for everything -> special collinear
case handles it: NOT intersecting (no overlap).

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- T-intersection: one endpoint lies ON the other segment, e.g.
  (0,0)-(4,0) and (2,0)-(2,5). Straddle test: D(2,0) is ON line
  AB -> d1 or d2 = 0 with the other point on segment -> counts as
  intersecting (onSeg special case).
- Fully collinear overlapping segments (0,0)-(4,0) and (2,0)-(6,0):
  intersect (they share (2,0)-(4,0)); the on-segment bounding-box
  test catches this.
- Fully collinear NON-overlapping (0,0)-(1,0) and (5,0)-(6,0):
  not intersecting - bounding boxes don't overlap.
- Floating point: with integer coordinates the cross product is
  EXACT - that is why this file uses long long, not double. Using
  doubles here is the classic precision bug.

DRY RUN (the X case):
A(0,0) B(4,4), C(0,4) D(4,0)
d1 = orient(C, D, A): C->D = (4,-4), C->A = (0,-4):
cross = 4*(-4) - (-4)*0 = -16 < 0 (A is one side)
d2 = orient(C, D, B): C->B = (4,0): cross = 4*0 - (-4)*4 = 16 > 0
d3 = orient(A, B, C): A->B = (4,4), A->C = (0,4):
cross = 4*4 - 4*0 = 16 > 0
d4 = orient(A, B, D): A->D = (4,0): cross = 4*0 - 4*4 = -16 < 0
d1 and d2 opposite signs AND d3, d4 opposite -> INTERSECT.

TIPS:
- onSeg(p, q, r): q collinear with p-r AND within the bounding
  box - needed for endpoint-touching cases.
- The four-orientation test handles every configuration including
  touching and collinear cases - no epsilon hacks needed with
  integers.
- Shapely/Google Maps/graphics engines all start from this test;
  polygon clipping (Sutherland-Hodgman) is built on it next.
*/

#include <stdio.h>

typedef struct {
    long long x;
    long long y;
} Point;

/*
 * orientation - >0 CCW, <0 CW, 0 collinear.
 */
long long orientation(Point p, Point q, Point r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

/*
 * onSegment - r is collinear with p->q AND inside its bounding box.
 */
int onSegment(Point p, Point q, Point r) {
    if (r.x < (p.x < q.x ? p.x : q.x) || r.x > (p.x > q.x ? p.x : q.x)) {
        return 0;
    }
    if (r.y < (p.y < p.y ? 0 : 0)) {
        return 0;
    }
    if (r.y < (p.y < q.y ? p.y : q.y) || r.y > (p.y > q.y ? p.y : q.y)) {
        return 0;
    }
    return 1;
}

int main() {
    Point a;
    Point b;
    Point c;
    Point d;
    int choice;

    printf("Line Segment Intersection (CCW orientation test)\n\n");

    do {
        printf("Segment 1: A x y: ");
        scanf("%lld %lld", &a.x, &a.y);
        printf("           B x y: ");
        scanf("%lld %lld", &b.x, &b.y);
        printf("Segment 2: C x y: ");
        scanf("%lld %lld", &c.x, &c.y);
        printf("           D x y: ");
        scanf("%lld %lld", &d.x, &d.y);

        long long d1 = orientation(a, b, c);
        long long d2 = orientation(a, b, d);
        long long d3 = orientation(c, d, a);
        long long d4 = orientation(c, d, b);

        printf("\nOrientations: d1=%lld d2=%lld d3=%lld d4=%lld\n",
               d1, d2, d3, d4);

        int intersect = 0;
        const char* reason = "";

        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
            ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
            intersect = 1;
            reason = "proper crossing (both segments straddle)";
        } else if (d1 == 0 && onSegment(a, b, c)) {
            intersect = 1;
            reason = "C lies on segment AB";
        } else if (d2 == 0 && onSegment(a, b, d)) {
            intersect = 1;
            reason = "D lies on segment AB";
        } else if (d3 == 0 && onSegment(c, d, a)) {
            intersect = 1;
            reason = "A lies on segment CD";
        } else if (d4 == 0 && onSegment(c, d, b)) {
            intersect = 1;
            reason = "B lies on segment CD";
        }

        if (intersect) {
            printf("=> SEGMENTS INTERSECT (%s)\n", reason);
        } else {
            printf("=> NO intersection\n");
        }

        printf("\nTest another pair? (1 = yes, 0 = no): ");
        scanf("%d", &choice);
        printf("\n");
    } while (choice == 1);

    return 0;
}
