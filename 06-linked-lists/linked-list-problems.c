/*
Linked List Problems Ladder (Basic -> Hard)
--------------------------------------------
Real life example:
Floyd's tortoise-and-hare cycle detection is used by garbage
collectors and duplicate finders; group reversal powers batch
processors that replay lists in chunks.

LADDER (easy -> hard inside this file):
  1. Find middle        (slow/fast pointers)
  2. Detect cycle       (Floyd's tortoise & hare)
  3. Merge two sorted lists (list version of merge step)
  4. Reverse in groups of k (advanced pointer surgery)

BEGINNER EXPLANATION:
- Middle: two runners - slow moves 1, fast moves 2. When fast
  hits the end, slow is at the middle (fast travels 2x distance).
- Cycle: same runners; if the list has a loop they MUST meet
  inside it (relative speed 1); if fast reaches NULL there is no
  loop.
- Merge: pick the smaller head repeatedly - identical to merge
  sort's merge step but rewiring pointers instead of copying.
- Group reverse: reverse k nodes, reconnect to the next group's
  result recursively/iteratively.

SUPER SIMPLE EXAMPLE (middle, 1->2->3->4->5):
slow: 1,2,3   fast: 1,3,5 -> fast NULL after 5; slow at 3. Correct!
Even length 1->2->3->4: slow 1,2,3? fast 1,3,NULL -> slow at 2
(first middle of two). Convention choice documented in code.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Empty list / single node: middle is the node itself; reverse-k
  needs k >= 2 to do anything (k=1 is identity).
- k > length: fewer than k nodes remain -> leave them as-is
  (standard definition; alternative: pad - say which you chose).
- Floyd meeting point: after meeting, reset one pointer to head
  and step BOTH by 1 - they re-meet exactly at the CYCLE START
  (the duplicate in the array version, folder 03).
- Merge with one list exhausted: attach the other list's rest.

DRY RUN (group reverse, 1->2->3->4->5, k=2):
group [1,2]: reverse -> 2->1; connect to result of next groups.
group [3,4]: reverse -> 4->3.
group [5]: <k nodes -> leave as-is.
Final: 2->1->4->3->5.

TIPS:
- Slow/fast pointer is a FAMILY: middle, cycle, kth-from-end,
  palindrome check - learn it once, reuse everywhere.
- Drawing pointer diagrams beats mental simulation for group
  reversal - paper is your debugger.
- These problems return in interview screens; cycle detection
  also powers the "duplicate number" trick (no extra space).
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    return node;
}

/*
 * buildList - Reads n values into a list.
 */
Node* buildList(int n) {
    Node* head = NULL;
    Node* tail = NULL;
    printf("Enter %d values: ", n);
    for (int i = 0; i < n; i++) {
        int v;
        scanf("%d", &v);
        Node* node = createNode(v);
        if (head == NULL) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }
    return head;
}

void printList(Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

/*
 * findMiddle - Slow/fast; even length returns the FIRST middle.
 */
Node* findMiddle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/*
 * hasCycle - Floyd's detection (works when you manually link a loop).
 */
int hasCycle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return 1;
        }
    }
    return 0;
}

/*
 * mergeSorted - Splices two sorted lists into one.
 */
Node* mergeSorted(Node* a, Node* b) {
    Node dummy;
    Node* tail = &dummy;
    dummy.next = NULL;

    while (a != NULL && b != NULL) {
        if (a->data <= b->data) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}

/*
 * reverseKGroup - Reverses list in groups of k; leftovers intact.
 */
Node* reverseKGroup(Node* head, int k) {
    if (k <= 1 || head == NULL) {
        return head;
    }
    /* check if k nodes exist */
    Node* check = head;
    for (int i = 0; i < k; i++) {
        if (check == NULL) {
            return head;
        }
        check = check->next;
    }

    Node* prev = NULL;
    Node* curr = head;
    int count = 0;
    while (count < k) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        count = count + 1;
    }
    head->next = reverseKGroup(curr, k);
    return prev;
}

void freeList(Node* head) {
    while (head != NULL) {
        Node* next = head->next;
        free(head);
        head = next;
    }
}

int main() {
    int choice;
    int n;

    printf("Linked List Problems Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. Find middle (slow/fast)\n");
        printf("2. Merge two sorted lists\n");
        printf("3. Reverse in groups of k\n");
        printf("4. Cycle detection demo (built-in loop)\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("How many values (1-50)? ");
            scanf("%d", &n);
            Node* head = buildList(n);
            printf("List: ");
            printList(head);
            Node* mid = findMiddle(head);
            printf("Middle value: %d\n", mid->data);
            freeList(head);
        } else if (choice == 2) {
            printf("List A size: ");
            scanf("%d", &n);
            Node* a = buildList(n);
            printf("List B size: ");
            scanf("%d", &n);
            Node* b = buildList(n);
            Node* merged = mergeSorted(a, b);
            printf("Merged: ");
            printList(merged);
            freeList(merged);
        } else if (choice == 3) {
            printf("How many values (1-50)? ");
            scanf("%d", &n);
            Node* head = buildList(n);
            printf("Group size k: ");
            scanf("%d", &n);
            head = reverseKGroup(head, n);
            printf("Group-reversed: ");
            printList(head);
            freeList(head);
        } else if (choice == 4) {
            printf("How many values (2-20)? ");
            scanf("%d", &n);
            Node* head = buildList(n);
            /* manually create a cycle: last node -> second node */
            Node* tail = head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = head->next;
            printf("Loop created (last -> second). hasCycle: %s\n",
                   hasCycle(head) ? "YES" : "NO");
            /* do not freeList - cyclic! break the loop first */
            tail->next = NULL;
            freeList(head);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
