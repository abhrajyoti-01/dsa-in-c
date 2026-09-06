/*
Queue Variants - Queue via 2 Stacks, Stack via 2 Queues (Basic -> Hard)
------------------------------------------------------------------------
Real life example:
Log pipelines buffer bursts with a queue; when only stack memory
is available (or an API exposes one primitive), the 2-stack trick
simulates FIFO with amortized O(1).

DIAGRAM (queue via 2 stacks):
  enqueue 1,2,3      in: [1 2 3]   out: [ ]
  dequeue: pour      out: [3 2 1] -> pop 1 (FIFO order!)
  dequeue again      out: [3 2]   -> pop 2, no pour needed
  each element moves at most twice -> amortized O(1).

LADDER (easy -> hard inside this file):
  1. Queue implemented with TWO STACKS  (amortized O(1))
  2. Stack implemented with TWO QUEUES  (push or pop O(n))

BEGINNER EXPLANATION:
- Queue from 2 stacks: stack A receives everything (in-stack).
  On dequeue, if out-stack is empty, pour A into B (reversing
  order). Dequeue from B. Each element is moved AT MOST TWICE in
  its lifetime -> amortized O(1) per operation even though one
  dequeue can cost O(n).
- Stack from 2 queues: keep ONE queue active. Push = enqueue.
  Pop = dequeue n-1 elements and re-enqueue them to the other
  queue; the LAST dequeued element is the answer (rotate the rest
  away). Either push-costly or pop-costly - you choose which.

SUPER SIMPLE EXAMPLE (queue via 2 stacks):
enqueue 1, 2, 3 -> in-stack: [1,2,3] (top=3)
dequeue -> out empty: pour: out [3,2,1]; pop 1. Returns 1. Correct FIFO!
dequeue -> out has [3,2] -> pop 2 directly. No pour needed.
enqueue 4 -> in [4]. dequeue -> out [3] -> pop 3; out empty.
dequeue -> pour 4 over -> returns 4.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- dequeue on BOTH stacks empty: report underflow.
- Pouring while out-stack NON-empty is a classic bug: leftover
  order would be destroyed. Pour ONLY when out is empty.
- Stack via 2 queues: if you rotate on PUSH (costly push, cheap
  pop) then dequeue of a just-pushed element is instant; the
  reverse style makes pop costly. Pick a lane - mixing them
  breaks the invariant.
- Amortized analysis: n enqueues + n dequeues = at most 2n moves
  total -> O(1) average per op despite occasional O(n) pour.

DRY RUN (stack via 2 queues, rotate-on-pop):
push 1 -> q1 [1]. push 2 -> q1 [1,2]. pop: move 1 to q2 ->
q1 [2] -> return 2; swap roles: q2 is now main [1].
push 3 -> main [1,3]. pop: move 1 -> aux; return 3.
LIFO order 2 then 3 - stack behavior achieved.

TIPS:
- "Queue from two stacks" is a top-3 interview question; the
  amortized argument is what they grade - memorize the 2-move
  lifetime bound.
- The pour-when-empty invariant is the whole correctness proof.
- These conversions prove ADTs are interfaces, not implementations:
  FIFO and LIFO can be built from each other.
*/

#include <stdio.h>

#define MAX 20

/* ---------- Queue via two stacks ---------- */
int inStack[MAX];
int outStack[MAX];
int inTop = -1;
int outTop = -1;

void qPush(int value) {
    if (inTop == MAX - 1) {
        printf("Queue full\n");
        return;
    }
    inTop = inTop + 1;
    inStack[inTop] = value;
    printf("enqueued %d (in-stack size %d)\n", value, inTop + 1);
}

void pourInToOut(void) {
    while (inTop >= 0) {
        outTop = outTop + 1;
        outStack[outTop] = inStack[inTop];
        inTop = inTop - 1;
    }
}

int qPop(void) {
    if (outTop == -1 && inTop == -1) {
        printf("Queue underflow\n");
        return -1;
    }
    if (outTop == -1) {
        pourInToOut();
        printf("poured in->out (reversed)\n");
    }
    int v = outStack[outTop];
    outTop = outTop - 1;
    printf("dequeued %d\n", v);
    return v;
}

/* ---------- Stack via two queues ---------- */
int q1[MAX];
int q2[MAX];
int q1Front = 0;
int q1Rear = -1;
int q2Front = 0;
int q2Rear = -1;

int sPushQ(int value) {
    if (q1Rear - q1Front + 1 == MAX) {
        printf("Stack full\n");
        return 0;
    }
    q1Rear = q1Rear + 1;
    q1[q1Rear] = value;
    printf("pushed %d\n", value);
    return 1;
}

int sPopQ(void) {
    int size1 = q1Rear - q1Front + 1;
    if (size1 <= 0) {
        printf("Stack underflow\n");
        return -1;
    }
    while (size1 > 1) {
        q2Rear = q2Rear + 1;
        q2[q2Rear] = q1[q1Front];
        q1Front = q1Front + 1;
        size1 = size1 - 1;
    }
    int v = q1[q1Front];
    q1Front = q1Front + 1;
    if (q1Front > q1Rear) {
        q1Front = 0;
        q1Rear = -1;
    }
    /* swap roles */
    int* tq = q1;
    int tf = q1Front;
    int tr = q1Rear;
    q1[0] = 0;
    q1Front = q2Front;
    q1Rear = q2Rear;
    for (int i = q2Front; i <= q2Rear; i++) {
        q1[i] = q2[i];
    }
    q2Front = tf;
    q2Rear = -1;
    (void)tq;
    (void)tr;
    printf("popped %d (LIFO!)\n", v);
    return v;
}

int main() {
    int choice;
    int value;

    printf("Queue/Stack Variants (easy -> hard)\n\n");

    while (1) {
        printf("\n-- Queue via 2 stacks --\n");
        printf("1. Enqueue\n2. Dequeue\n");
        printf("-- Stack via 2 queues --\n");
        printf("3. Push\n4. Pop\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Value: ");
            scanf("%d", &value);
            qPush(value);
        } else if (choice == 2) {
            qPop();
        } else if (choice == 3) {
            printf("Value: ");
            scanf("%d", &value);
            sPushQ(value);
        } else if (choice == 4) {
            sPopQ();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
