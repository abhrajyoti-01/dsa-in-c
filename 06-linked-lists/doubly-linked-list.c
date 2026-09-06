/*
Doubly Linked List
------------------
What this program solves:
Menu-driven doubly linked list: each node has prev AND next
pointers, allowing traversal in both directions, O(1) delete of a
known node, and forward/backward display.

Real life example:
A music playlist with Previous and Next buttons - each song knows
both its neighbours, so you can skip either way. Browser history
Back/Forward works the same way.

DIAGRAM (each node knows both neighbours):
  NULL <- [10] <-> [20] <-> [30] -> NULL
  delete 20: 10->next = 30 AND 30->prev = 10  (two links, one node)
  no traversal needed to reach the previous node.

How the process works:
1. Node = prev + data + next; head->prev = NULL, tail->next = NULL.
2. Insert head: node->next = head; head->prev = node; head = node.
3. Insert tail: tail->next = node; node->prev = tail; tail = node.
4. Delete: victim->prev->next = victim->next;
   victim->next->prev = victim->prev (fix BOTH sides), then free.

Pseudo code:
START
INSERT_HEAD(x): node.prev = NULL; node.next = head;
    head.prev = node; head = node
DELETE(victim): victim.prev.next = victim.next
    victim.next.prev = victim.prev
END

Sample input and dry run:
insert tail 1, 2, 3 -> 1 <-> 2 <-> 3
forward:  1 2 3
backward: 3 2 1
delete 2  -> 1 <-> 3

Main logic to understand:
Two pointers per node buy bidirectional traversal and O(1) deletion
given the node pointer - the cost is one extra pointer of memory
per node and slightly trickier bookkeeping on every operation.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

DNode* head = NULL;
DNode* tail = NULL;

/*
 * createDNode - Allocates a doubly-linked node.
 */
DNode* createDNode(int value) {
    DNode* node = (DNode*)malloc(sizeof(DNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->data = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/*
 * insertHead - O(1) insertion at the front.
 */
void insertHead(int value) {
    DNode* node = createDNode(value);
    node->next = head;
    if (head != NULL) {
        head->prev = node;
    } else {
        tail = node;
    }
    head = node;
    printf("Inserted %d at head\n", value);
}

/*
 * insertTail - O(1) insertion at the back (tail pointer).
 */
void insertTail(int value) {
    DNode* node = createDNode(value);
    node->prev = tail;
    if (tail != NULL) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    printf("Inserted %d at tail\n", value);
}

/*
 * deleteValue - Removes first occurrence, fixing both sides.
 */
void deleteValue(int value) {
    DNode* curr = head;
    while (curr != NULL && curr->data != value) {
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("%d not found\n", value);
        return;
    }
    if (curr->prev != NULL) {
        curr->prev->next = curr->next;
    } else {
        head = curr->next;
    }
    if (curr->next != NULL) {
        curr->next->prev = curr->prev;
    } else {
        tail = curr->prev;
    }
    free(curr);
    printf("Deleted %d\n", value);
}

/*
 * displayForward - Head to tail.
 */
void displayForward(void) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    printf("Forward:  ");
    for (DNode* curr = head; curr != NULL; curr = curr->next) {
        printf("%d ", curr->data);
    }
    printf("\n");
}

/*
 * displayBackward - Tail to head (proves prev pointers work).
 */
void displayBackward(void) {
    if (tail == NULL) {
        printf("List is empty\n");
        return;
    }
    printf("Backward: ");
    for (DNode* curr = tail; curr != NULL; curr = curr->prev) {
        printf("%d ", curr->data);
    }
    printf("\n");
}

int main() {
    int choice;
    int value;

    while (1) {
        printf("\nDoubly Linked List\n");
        printf("1. Insert at head\n");
        printf("2. Insert at tail\n");
        printf("3. Delete by value\n");
        printf("4. Display forward\n");
        printf("5. Display backward\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter value: ");
            scanf("%d", &value);
            insertHead(value);
        } else if (choice == 2) {
            printf("Enter value: ");
            scanf("%d", &value);
            insertTail(value);
        } else if (choice == 3) {
            printf("Enter value to delete: ");
            scanf("%d", &value);
            deleteValue(value);
        } else if (choice == 4) {
            displayForward();
        } else if (choice == 5) {
            displayBackward();
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    /* STEP: free every node before exit */
    while (head != NULL) {
        DNode* next = head->next;
        free(head);
        head = next;
    }

    return 0;
}
