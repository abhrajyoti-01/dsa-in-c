/*
Circular Linked List
--------------------
What this program solves:
Menu-driven circular singly linked list: the last node points back
to the head, forming a ring. Supports insert head/tail, delete,
search, and display (any node can reach every other node).

Real life example:
A group of players sitting in a circle passing a ball - after the
last player the ball returns to the first. Round-robin CPU
scheduling uses exactly this structure.

DIAGRAM (last node points BACK to the first):
  +--> [10] --> [20] --> [30] --+
  |                             |
  +-----------------------------+
  walk with a temp pointer until temp->next == head again;
  the list has NO NULL at the end.

How the process works:
1. Keep only a TAIL pointer; tail->next is the head.
2. Insert head: node->next = tail->next; tail->next = node.
3. Insert tail: same as head insert, then move tail to node.
4. Delete: walk the ring to find the previous node of the victim,
   re-route, free. If the victim was the tail, update tail.
5. Display: start at tail->next and stop when you come back around.

Pseudo code:
START
INSERT_TAIL(x): node.next = tail.next; tail.next = node; tail = node
DISPLAY: curr = tail.next
    DO print curr; curr = curr.next WHILE curr != tail.next
END

Sample input and dry run:
insert tail 1, 2, 3 -> ring 1->2->3->1
display: 1 2 3 (then back to 1)
delete 2 -> ring 1->3->1

Main logic to understand:
No NULL terminator: traversal uses a do-while and the tail pointer.
O(1) insert at both ends (only tail is tracked). Round-robin loops
never need to reset to head.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct CNode {
    int data;
    struct CNode* next;
} CNode;

CNode* tail = NULL;

/*
 * createCNode - Allocates a circular node.
 */
CNode* createCNode(int value) {
    CNode* node = (CNode*)malloc(sizeof(CNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->data = value;
    node->next = NULL;
    return node;
}

/*
 * insertHead - New node becomes the head (tail->next).
 */
void insertHead(int value) {
    CNode* node = createCNode(value);
    if (tail == NULL) {
        node->next = node;
        tail = node;
    } else {
        node->next = tail->next;
        tail->next = node;
    }
    printf("Inserted %d at head\n", value);
}

/*
 * insertTail - Insert at head position, then advance tail.
 */
void insertTail(int value) {
    CNode* node = createCNode(value);
    if (tail == NULL) {
        node->next = node;
    } else {
        node->next = tail->next;
        tail->next = node;
    }
    tail = node;
    printf("Inserted %d at tail\n", value);
}

/*
 * deleteValue - Removes first occurrence from the ring.
 */
void deleteValue(int value) {
    if (tail == NULL) {
        printf("List is empty\n");
        return;
    }
    CNode* curr = tail->next;
    CNode* prev = tail;
    do {
        if (curr->data == value) {
            if (curr == prev) {
                tail = NULL;
            } else {
                prev->next = curr->next;
                if (curr == tail) {
                    tail = prev;
                }
            }
            free(curr);
            printf("Deleted %d\n", value);
            return;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != tail->next);
    printf("%d not found\n", value);
}

/*
 * searchValue - Walks the ring once looking for the value.
 */
void searchValue(int value) {
    if (tail == NULL) {
        printf("List is empty\n");
        return;
    }
    CNode* curr = tail->next;
    int pos = 1;
    do {
        if (curr->data == value) {
            printf("%d found at position %d\n", value, pos);
            return;
        }
        curr = curr->next;
        pos = pos + 1;
    } while (curr != tail->next);
    printf("%d not found\n", value);
}

/*
 * display - Traverses the ring exactly once using do-while.
 */
void display(void) {
    if (tail == NULL) {
        printf("List is empty\n");
        return;
    }
    printf("Ring: ");
    CNode* curr = tail->next;
    do {
        printf("%d -> ", curr->data);
        curr = curr->next;
    } while (curr != tail->next);
    printf("(back to head)\n");
}

int main() {
    int choice;
    int value;

    while (1) {
        printf("\nCircular Linked List\n");
        printf("1. Insert at head\n");
        printf("2. Insert at tail\n");
        printf("3. Delete by value\n");
        printf("4. Search\n");
        printf("5. Display\n");
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
            printf("Enter value to search: ");
            scanf("%d", &value);
            searchValue(value);
        } else if (choice == 5) {
            display();
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    /* STEP: break the ring, then free node by node */
    if (tail != NULL) {
        CNode* curr = tail->next;
        tail->next = NULL;
        while (curr != NULL) {
            CNode* next = curr->next;
            free(curr);
            curr = next;
        }
    }

    return 0;
}
