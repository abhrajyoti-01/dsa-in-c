/*
Queue (FIFO - First In, First Out)
----------------------------------
What this program solves:
Menu-driven implementation of a Queue using a fixed-size array with
user input for enqueue, dequeue, peek, display, and overflow/
underflow handling. Supports both LINEAR and CIRCULAR modes to show
why the circular queue fixes the wasted-space problem.

Real life example:
A ticket counter line. The person who joins first is served first
(FIFO). New people join at the rear, and service happens at the
front. Printers, CPU scheduling, and call centers all use queues.

DIAGRAM (linear queue, MAX 5, after enqueue 10,20,30 + dequeue 10):
        front=1              rear=2
          |                    |
          v                    v
        +----+----+----+----+----+
        | .. | 20 | 30 | .. | .. |
        +----+----+----+----+----+
          0     1     2    3    4   (slot 0 wasted - linear flaw)

CIRCULAR queue: rear wraps (rear+1) % MAX, so slot 0 is reused:
        +----+----+----+----+----+
        | 60 | 20 | 30 | 40 | 50 |   60 entered slot 0 after wrap
        +----+----+----+----+----+
          ^front=1        rear wraps to 0
        Full when (rear+1) % MAX == front.

How the process works:
1. front and rear start at -1 (empty queue).
2. ENQUEUE (linear): if rear reaches MAX-1 the queue is FULL.
   On the very first element front becomes 0; rear increments and
   the value is stored at queue[rear].
3. DEQUEUE: the value at queue[front] leaves. If front passes rear,
   the queue is empty again (front = rear = -1).
4. LINEAR DRAWBACK: after dequeues, cells before front can never be
   reused even though they are free.
5. CIRCULAR FIX: rear/front wrap around using (index + 1) % MAX,
   so freed cells are reused. Full test: (rear+1)%MAX == front.

Pseudo code:
START
ENQUEUE(value)
    If rear == MAX-1: report overflow, stop
    If front == -1: front = 0
    rear = rear + 1
    queue[rear] = value
DEQUEUE()
    If front == -1: report underflow, stop
    value = queue[front]
    front = front + 1
    If front > rear: front = rear = -1
END

Sample input and dry run (MAX = 5):
Choice 1, values 10 20 30 -> [10, 20, 30]  front=0, rear=2
Choice 2                  -> removes 10, front=1
Choice 4                  -> prints 20 30
Choice 3                  -> Front element: 20
Choice 6                  -> exit

Main logic to understand:
Insertion happens only at the rear and deletion only at the front,
so both operations are O(1). The circular version turns the array
into a ring so no space is ever wasted.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 5

int queue[MAX];
int front = -1;
int rear = -1;

/*
 * isEmpty - Returns 1 when the queue has no elements.
 */
int isEmpty() {
    return front == -1;
}

/*
 * isFullLinear - Linear queue is full when rear hits the last cell.
 */
int isFullLinear() {
    return rear == MAX - 1;
}

/*
 * enqueue - Adds a value at the rear of the linear queue.
 */
void enqueue(int value) {
    if (isFullLinear()) {
        printf("Overflow! Cannot enqueue %d (queue is full)\n", value);
        return;
    }
    if (front == -1) {
        front = 0;
    }
    rear = rear + 1;
    queue[rear] = value;
    printf("Enqueued %d\n", value);
}

/*
 * dequeue - Removes the value at the front of the linear queue.
 */
int dequeue() {
    if (isEmpty()) {
        printf("Underflow! Cannot dequeue (queue is empty)\n");
        return -1;
    }
    int value = queue[front];
    front = front + 1;
    if (front > rear) {
        front = -1;
        rear = -1;
    }
    printf("Dequeued %d\n", value);
    return value;
}

/*
 * peek - Shows the front value without removing it.
 */
void peek() {
    if (isEmpty()) {
        printf("Queue is empty\n");
    } else {
        printf("Front element: %d\n", queue[front]);
    }
}

/*
 * display - Prints the queue from front to rear.
 */
void display() {
    if (isEmpty()) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue (front -> rear): ");
    for (int i = front; i <= rear; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}

/*
 * Circular queue section - reuses freed cells by wrapping around.
 */
int cq[MAX];
int cqFront = -1;
int cqRear = -1;

/*
 * isFullCircular - Full when the next slot after rear is front.
 */
int isFullCircular() {
    return (cqRear + 1) % MAX == cqFront;
}

/*
 * isEmptyCircular - Returns 1 when the circular queue is empty.
 */
int isEmptyCircular() {
    return cqFront == -1;
}

/*
 * enqueueCircular - Adds a value, wrapping rear around the ring.
 */
void enqueueCircular(int value) {
    if (isFullCircular()) {
        printf("Circular overflow! Cannot enqueue %d\n", value);
        return;
    }
    if (cqFront == -1) {
        cqFront = 0;
    }
    cqRear = (cqRear + 1) % MAX;
    cq[cqRear] = value;
    printf("Enqueued %d (rear now at slot %d)\n", value, cqRear);
}

/*
 * dequeueCircular - Removes the front value, wrapping front around.
 */
int dequeueCircular() {
    if (isEmptyCircular()) {
        printf("Circular underflow! Queue is empty\n");
        return -1;
    }
    int value = cq[cqFront];
    if (cqFront == cqRear) {
        cqFront = -1;
        cqRear = -1;
    } else {
        cqFront = (cqFront + 1) % MAX;
    }
    printf("Dequeued %d\n", value);
    return value;
}

/*
 * displayCircular - Prints elements in logical (front -> rear) order.
 */
void displayCircular() {
    if (isEmptyCircular()) {
        printf("Circular queue is empty\n");
        return;
    }
    printf("Circular queue (front -> rear): ");
    int i = cqFront;
    while (1) {
        printf("%d ", cq[i]);
        if (i == cqRear) {
            break;
        }
        i = (i + 1) % MAX;
    }
    printf("\n");
}

int main() {
    int choice, value;

    while (1) {
        printf("\nQueue (Array Implementation) - FIFO\n");
        printf("1. Enqueue (linear)\n");
        printf("2. Dequeue (linear)\n");
        printf("3. Peek front (linear)\n");
        printf("4. Display linear queue\n");
        printf("5. Enqueue (circular)\n");
        printf("6. Dequeue (circular)\n");
        printf("7. Display circular queue\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter value to enqueue: ");
            scanf("%d", &value);
            enqueue(value);
        } else if (choice == 2) {
            dequeue();
        } else if (choice == 3) {
            peek();
        } else if (choice == 4) {
            display();
        } else if (choice == 5) {
            printf("Enter value to enqueue: ");
            scanf("%d", &value);
            enqueueCircular(value);
        } else if (choice == 6) {
            dequeueCircular();
        } else if (choice == 7) {
            displayCircular();
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}