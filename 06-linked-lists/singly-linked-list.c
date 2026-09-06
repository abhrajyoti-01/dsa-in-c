/*
Singly Linked List
------------------
What this program solves:
Menu-driven singly linked list: insert at head/tail/position,
delete by value or position, search, reverse, and display.

Real life example:
A treasure hunt: each clue (node) contains the prize hint (data)
and the location of the NEXT clue (pointer). You can only move
forward, and inserting a new clue just means re-routing one arrow.

DIAGRAM (list 5 -> 10 -> 20, inserting 15 at position 2):
Before:
  head
   |
   v
 +----+    +----+    +----+
 | 5  |--->| 10 |--->| 20 |---> NULL
 +----+    +----+    +----+
              ^ new 15 goes here (after 10)

DIAGRAM (nodes chained by pointers, O(1) insert at head):
  head -> [10|next] -> [20|next] -> [30|NULL]
  insert 5 at head: 5->next = head; head = 5
  delete 20: walk until prev->next == 20, relink prev->next = 20->next

Steps: newNode->next = curr10->next;  curr10->next = newNode
After:
 +----+    +----+    +----+    +----+
 | 5  |--->| 10 |--->| 15 |--->| 20 |---> NULL
 +----+    +----+    +----+    +----+

REVERSE (three runners prev/curr/next):
NULL <- [5] <- [10] <- [20]   head moves to old tail.

How the process works:
1. Node = data + next pointer; head points to the first node.
2. Insert at head: newNode->next = head; head = newNode. O(1).
3. Insert at tail: walk to the last node, link it. O(n).
4. Delete: re-route the previous node's next around the victim,
   free() the victim.
5. Reverse: flip each next pointer using three runners
   (prev, curr, next).

Pseudo code:
START
INSERT_HEAD(x): node.next = head; head = node
DELETE(x): find node; prev.next = node.next; free(node)
REVERSE: prev = NULL
    WHILE curr: next = curr.next; curr.next = prev;
                prev = curr; curr = next
    head = prev
END

Sample input and dry run:
insert tail 10 -> head -> 10
insert tail 20 -> head -> 10 -> 20
insert head 5  -> head -> 5 -> 10 -> 20
delete 10      -> head -> 5 -> 20

Main logic to understand:
Linked lists trade O(1) random access for O(1) insertion/deletion
at a known point. No shifting like arrays; memory grows node by
node. Singly = one direction only; reverse in O(n) with O(1) extra.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;

/*
 * createNode - Allocates a node with the given value.
 */
Node* createNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->data = value;
    node->next = NULL;
    return node;
}

/*
 * insertHead - O(1) insertion at the front.
 */
void insertHead(int value) {
    Node* node = createNode(value);
    node->next = head;
    head = node;
    printf("Inserted %d at head\n", value);
}

/*
 * insertTail - Walks to the end, links the new node. O(n).
 */
void insertTail(int value) {
    Node* node = createNode(value);
    if (head == NULL) {
        head = node;
    } else {
        Node* curr = head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = node;
    }
    printf("Inserted %d at tail\n", value);
}

/*
 * insertAtPosition - 1-based position; position 1 = head.
 */
void insertAtPosition(int value, int pos) {
    if (pos < 1) {
        printf("Invalid position\n");
        return;
    }
    if (pos == 1) {
        insertHead(value);
        return;
    }
    Node* curr = head;
    for (int i = 1; i < pos - 1 && curr != NULL; i++) {
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Position beyond list length\n");
        return;
    }
    Node* node = createNode(value);
    node->next = curr->next;
    curr->next = node;
    printf("Inserted %d at position %d\n", value, pos);
}

/*
 * deleteValue - Removes the FIRST node holding the value.
 */
void deleteValue(int value) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    if (head->data == value) {
        Node* victim = head;
        head = head->next;
        free(victim);
        printf("Deleted %d\n", value);
        return;
    }
    Node* curr = head;
    while (curr->next != NULL && curr->next->data != value) {
        curr = curr->next;
    }
    if (curr->next == NULL) {
        printf("%d not found\n", value);
        return;
    }
    Node* victim = curr->next;
    curr->next = victim->next;
    free(victim);
    printf("Deleted %d\n", value);
}

/*
 * searchValue - Reports the 1-based position of the value.
 */
void searchValue(int value) {
    Node* curr = head;
    int pos = 1;
    while (curr != NULL) {
        if (curr->data == value) {
            printf("%d found at position %d\n", value, pos);
            return;
        }
        curr = curr->next;
        pos = pos + 1;
    }
    printf("%d not found\n", value);
}

/*
 * reverseList - Flips all next pointers in one pass.
 */
void reverseList(void) {
    Node* prev = NULL;
    Node* curr = head;
    while (curr != NULL) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
    printf("List reversed\n");
}

/*
 * display - Prints the chain with arrows.
 */
void display(void) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    printf("head -> ");
    for (Node* curr = head; curr != NULL; curr = curr->next) {
        printf("%d -> ", curr->data);
    }
    printf("NULL\n");
}

int main() {
    int choice;
    int value;
    int pos;

    while (1) {
        printf("\nSingly Linked List\n");
        printf("1. Insert at head\n");
        printf("2. Insert at tail\n");
        printf("3. Insert at position\n");
        printf("4. Delete by value\n");
        printf("5. Search\n");
        printf("6. Reverse list\n");
        printf("7. Display\n");
        printf("8. Exit\n");
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
            printf("Enter value and position: ");
            scanf("%d %d", &value, &pos);
            insertAtPosition(value, pos);
        } else if (choice == 4) {
            printf("Enter value to delete: ");
            scanf("%d", &value);
            deleteValue(value);
        } else if (choice == 5) {
            printf("Enter value to search: ");
            scanf("%d", &value);
            searchValue(value);
        } else if (choice == 6) {
            reverseList();
        } else if (choice == 7) {
            display();
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    /* STEP: free every node before exit */
    while (head != NULL) {
        Node* next = head->next;
        free(head);
        head = next;
    }

    return 0;
}
