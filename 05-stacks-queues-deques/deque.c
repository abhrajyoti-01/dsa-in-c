/*
Deque (Double-Ended Queue)
--------------------------
What this program solves:
Menu-driven deque on a circular array: insert and remove at BOTH
the front and the rear, plus peek at both ends.

Real life example:
A checkout line with both doors open: VIPs enter from the FRONT
(cutting in politely is allowed here), normal customers join at the
REAR, and service can leave from either end. Browser history,
undo-redo stacks, and sliding-window problems use deques.

DIAGRAM (deque = double-ended queue, both ends open):
  push/pop            push/pop
     v                   v
  +----+----+----+----+----+
  | 20 | 30 | 40 | 50 | .. |
  +----+----+----+----+----+
     front ----> rear
  circular array: (rear + 1) % MAX keeps the ends joined.

How the process works:
1. A circular array with front, rear, and size counters.
2. insertFront: front = (front - 1 + MAX) % MAX; store; size++.
3. insertRear:  rear = (rear + 1) % MAX; store; size++.
4. deleteFront: read front, front = (front + 1) % MAX; size--.
5. deleteRear:  read rear, rear = (rear - 1 + MAX) % MAX; size--.
6. Full when size == MAX; empty when size == 0.

Pseudo code:
START
INSERT_FRONT(x): IF full: overflow
    front = (front-1+MAX) % MAX; dq[front] = x; size++
INSERT_REAR(x): IF full: overflow
    rear = (rear+1) % MAX; dq[rear] = x; size++
DELETE_FRONT: x = dq[front]; front = (front+1)%MAX; size--
DELETE_REAR:  x = dq[rear];  rear = (rear-1+MAX)%MAX; size--
END

Sample input and dry run (MAX 5):
insertRear 10, 20 -> [10, 20]
insertFront 5     -> [5, 10, 20]
deleteRear        -> removes 20 -> [5, 10]
insertFront 2     -> [2, 5, 10]

Main logic to understand:
A deque = stack + queue in one: push/pop at the rear is a stack,
enqueue rear + dequeue front is a queue. All four operations are
O(1) thanks to the circular wrap-around with modulo arithmetic.
*/

#include <stdio.h>

#define MAX 5

int dq[MAX];
int front = 0;
int rear = -1;
int size = 0;

/*
 * isEmpty - Returns 1 when the deque holds no elements.
 */
int isEmpty(void) {
    return size == 0;
}

/*
 * isFull - Returns 1 when MAX elements are stored.
 */
int isFull(void) {
    return size == MAX;
}

/*
 * insertFront - Adds at the front, wrapping backwards.
 */
void insertFront(int value) {
    if (isFull()) {
        printf("Overflow! Deque is full\n");
        return;
    }
    front = (front - 1 + MAX) % MAX;
    dq[front] = value;
    size = size + 1;
    if (rear == -1) {
        rear = front;
    }
    printf("Inserted %d at front\n", value);
}

/*
 * insertRear - Adds at the rear, wrapping forward.
 */
void insertRear(int value) {
    if (isFull()) {
        printf("Overflow! Deque is full\n");
        return;
    }
    rear = (rear + 1) % MAX;
    dq[rear] = value;
    size = size + 1;
    printf("Inserted %d at rear\n", value);
}

/*
 * deleteFront - Removes from the front.
 */
void deleteFront(void) {
    if (isEmpty()) {
        printf("Underflow! Deque is empty\n");
        return;
    }
    printf("Deleted %d from front\n", dq[front]);
    front = (front + 1) % MAX;
    size = size - 1;
}

/*
 * deleteRear - Removes from the rear.
 */
void deleteRear(void) {
    if (isEmpty()) {
        printf("Underflow! Deque is empty\n");
        return;
    }
    printf("Deleted %d from rear\n", dq[rear]);
    rear = (rear - 1 + MAX) % MAX;
    size = size - 1;
}

/*
 * peekFront / peekRear - Show end values without removing.
 */
void peekFront(void) {
    if (isEmpty()) {
        printf("Deque is empty\n");
    } else {
        printf("Front element: %d\n", dq[front]);
    }
}

void peekRear(void) {
    if (isEmpty()) {
        printf("Deque is empty\n");
    } else {
        printf("Rear element: %d\n", dq[rear]);
    }
}

/*
 * display - Prints from front to rear in logical order.
 */
void display(void) {
    if (isEmpty()) {
        printf("Deque is empty\n");
        return;
    }
    printf("Deque (front -> rear): ");
    int i = front;
    for (int c = 0; c < size; c++) {
        printf("%d ", dq[i]);
        i = (i + 1) % MAX;
    }
    printf("\n");
}

int main() {
    int choice;
    int value;

    while (1) {
        printf("\nDeque (Circular Array, capacity %d)\n", MAX);
        printf("1. Insert at front\n");
        printf("2. Insert at rear\n");
        printf("3. Delete from front\n");
        printf("4. Delete from rear\n");
        printf("5. Peek front\n");
        printf("6. Peek rear\n");
        printf("7. Display\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1 || choice == 2) {
            printf("Enter value: ");
            scanf("%d", &value);
            if (choice == 1) {
                insertFront(value);
            } else {
                insertRear(value);
            }
        } else if (choice == 3) {
            deleteFront();
        } else if (choice == 4) {
            deleteRear();
        } else if (choice == 5) {
            peekFront();
        } else if (choice == 6) {
            peekRear();
        } else if (choice == 7) {
            display();
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
