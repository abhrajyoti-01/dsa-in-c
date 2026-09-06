/*
Red-Black Tree
--------------
What this program solves:
This program implements a Red-Black Tree, a self-balancing Binary Search
Tree where every node is colored red or black and five color rules keep
the tree roughly balanced. It supports insertion and inorder traversal.

Real life example:
The Linux kernel schedules thousands of processes by keeping them in a
Red-Black Tree ordered by "virtual runtime". The next process to run is
always the leftmost node, found in O(log n). The color rules are like a
dress code at a club that forces the crowd to spread out evenly instead
of piling onto one side.

DIAGRAM (red-black rules bound the height by 2*log2(n+1)):
  rule 1: every root->leaf path has the same number of BLACK nodes
  rule 2: a red node never has a red child
  insert as RED, then fix red-red clashes by recoloring/rotating:
      B                B
       \                \
        R     ->         R          recolor uncle red, or
       /                / \         rotate and swap colors
      R                R   R

How the process works:
1. Insert the new node as in a normal BST, colored RED.
2. If its parent is black, nothing more is needed.
3. If its parent is red, two reds touch (a violation). Look at the uncle:
   - Uncle red: repaint parent and uncle black, grandparent red, move up.
   - Uncle black: perform rotations (left or right) and repaint.
4. Finally, force the root to black.

Pseudo code:
START
Insert(root, key)
    Insert as in BST with color RED
    While parent of node is RED
        If parent is left child of grandparent
            If uncle is RED: recolor parent, uncle; grandparent RED; move up
            Else: handle LR triangle then LL line with rotations and recolor
        Else mirror the same cases
    Color root BLACK
END

Sample input and dry run:
Insert 10, 20, 30
After inserting 30: 10(B) -> 20(R) -> 30(R) violates the rules.
A left rotation at 10 with recoloring gives root 20(B) with children
10(R) and 30(R). Inorder still prints 10 20 30.

Main logic to understand:
The black-height rule (every path has the same number of black nodes)
bounds the longest path at twice the shortest, so the tree height is
always O(log n) - search, insert and delete never degrade.
*/
#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

Node* NIL;

Node* createNode(int value, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->color = RED;
    newNode->parent = parent;
    newNode->left = NIL;
    newNode->right = NIL;
    return newNode;
}

void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(Node** root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != NIL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NIL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void insertFixup(Node** root, Node* node) {
    while (node->parent->color == RED) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    leftRotate(root, node);
                    parent = node->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(root, grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rightRotate(root, node);
                    parent = node->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                leftRotate(root, grandparent);
            }
        }
    }
    (*root)->color = BLACK;
}

void insert(Node** root, int value) {
    Node* parent = NIL;
    Node* current = *root;

    while (current != NIL) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        } else if (value > current->data) {
            current = current->right;
        } else {
            return;
        }
    }

    Node* newNode = createNode(value, parent);

    if (parent == NIL) {
        *root = newNode;
    } else if (value < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    insertFixup(root, newNode);
}

Node* search(Node* root, int target) {
    while (root != NIL && root->data != target) {
        if (target < root->data) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}

void inorder(Node* root) {
    if (root != NIL) {
        inorder(root->left);
        printf("%d(%c) ", root->data, root->color == RED ? 'R' : 'B');
        inorder(root->right);
    }
}

int main() {
    Node* root = NIL;
    int n, value;

    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->parent = NIL;
    NIL->left = NIL;
    NIL->right = NIL;

    printf("Red-Black Tree\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        insert(&root, value);
    }

    printf("Inorder with colors: ");
    inorder(root);
    printf("\n");

    printf("Enter a value to search: ");
    scanf("%d", &value);
    if (search(root, value) != NIL) {
        printf("%d found in the tree\n", value);
    } else {
        printf("%d not found\n", value);
    }

    return 0;
}
