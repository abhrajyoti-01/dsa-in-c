/*
Threaded Binary Tree
--------------------
What this program solves:
This program builds a doubly threaded binary tree where NULL right
pointers link to the inorder successor and NULL left pointers link to
the inorder predecessor. Inorder traversal then needs no recursion and
no stack.

Real life example:
Think of a museum tour where unused doors between halls are converted
into one-way corridors that always lead to the next hall in tour order.
A visitor can walk the entire tour by following corridors - no map and
no backtracking needed. That is exactly what threads provide.

DIAGRAM (null pointers become threads to the in-order neighbour):
        50
       /  \
      30   70
       \__thread-> 50   (successor thread)
  left thread = predecessor, right thread = successor;
  in-order traversal without recursion and without a stack.

How the process works:
1. Insert nodes as in a BST.
2. When attaching a new node where a pointer used to be a thread, the
   new node's left thread points to its predecessor and its right
   thread points to its successor.
3. To traverse inorder: go to the leftmost node, then repeatedly follow
   right threads; when a right pointer is a real child, jump to the
   leftmost node of that subtree.

Pseudo code:
START
Inorder(root)
    node = leftmost node of root
    While node is not NULL
        Print node
        If node.right is a thread: node = node.right
        Else: node = leftmost node of node.right subtree

leftmost(node)
    While node has a real left child: node = node.left
    Return node
END

Sample input and dry run:
Insert 50, 30, 70, 20, 40
Tree: 50 root, 30 left, 70 right, 20 left of 30, 40 right of 30.
Threads: 20.right -> 30, 40.right -> 50, 30.left -> 20, 70.left -> 50.
Inorder walk: 20 -> 30 -> 40 -> 50 -> 70 using only threads and one
leftmost jump, with zero stack usage.

Main logic to understand:
A tree with n nodes has n+1 NULL pointers; threading reuses them to
store the inorder chain, turning an O(n) space traversal into O(1).
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct ThreadedNode {
    int data;
    struct ThreadedNode* left;
    struct ThreadedNode* right;
    int leftIsThread;
    int rightIsThread;
} ThreadedNode;

ThreadedNode* createNode(int value) {
    ThreadedNode* newNode = (ThreadedNode*)malloc(sizeof(ThreadedNode));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->leftIsThread = 1;
    newNode->rightIsThread = 1;
    return newNode;
}

ThreadedNode* insert(ThreadedNode* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    ThreadedNode* current = root;
    ThreadedNode* newNode = createNode(value);

    while (1) {
        if (value < current->data) {
            if (current->leftIsThread) {
                newNode->left = current->left;
                newNode->right = current;
                current->leftIsThread = 0;
                current->left = newNode;
                return root;
            }
            current = current->left;
        } else if (value > current->data) {
            if (current->rightIsThread) {
                newNode->right = current->right;
                newNode->left = current;
                current->rightIsThread = 0;
                current->right = newNode;
                return root;
            }
            current = current->right;
        } else {
            free(newNode);
            return root;
        }
    }
}

ThreadedNode* leftmost(ThreadedNode* node) {
    if (node == NULL) {
        return NULL;
    }
    while (node->leftIsThread == 0) {
        node = node->left;
    }
    return node;
}

void inorder(ThreadedNode* root) {
    ThreadedNode* current = leftmost(root);
    while (current != NULL) {
        printf("%d ", current->data);
        if (current->rightIsThread) {
            current = current->right;
        } else {
            current = leftmost(current->right);
        }
    }
}

int main() {
    ThreadedNode* root = NULL;
    int n, value;

    printf("Threaded Binary Tree\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }

    printf("Inorder traversal (no stack, no recursion): ");
    inorder(root);
    printf("\n");

    return 0;
}
