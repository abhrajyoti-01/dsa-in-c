/*
AVL Tree
--------
What this program solves:
This program implements a self-balancing Binary Search Tree. After every
insertion the tree checks its balance and performs rotations so the
height always stays around log n, guaranteeing fast search.

Real life example:
A regular BST can become a long chain if values arrive in sorted order,
making searches slow. An AVL tree is like a waiter constantly rearranging
a stack of trays so it never leans too much to one side. Whenever one
side gets heavier by more than one level, the tree rotates to balance.

DIAGRAM (RR case - one left rotation restores balance):
  insert 30, 40, 50:
    30                       40
      \         rotate L    /  \
       40      ------->    30    50
         \
          50
  balance factor = height(L) - height(R) must stay in {-1, 0, 1}.

How the process works:
1. Insert the value as in a normal BST.
2. While returning back up, update the height of each node.
3. Compute balance factor = height(left) - height(right).
4. If balance factor is not in {-1, 0, 1}, rotate:
   LL case -> right rotation
   RR case -> left rotation
   LR case -> left rotation on child, then right rotation
   RL case -> right rotation on child, then left rotation

Pseudo code:
START
Insert(node, value)
    If node is NULL return new node
    Insert into left or right subtree as in BST
    Update height of node
    balance = height(left) - height(right)
    If balance > 1 and value < left data: right rotate
    If balance < -1 and value > right data: left rotate
    If balance > 1 and value > left data: left rotate child then right rotate
    If balance < -1 and value < right data: right rotate child then left rotate
END

Sample input and dry run:
Insert 10, 20, 30
After 30 the tree leans right (10 -> 20 -> 30).
Balance factor of 10 becomes -2, so a left rotation at 10 makes 20 the
root with children 10 and 30. Tree stays balanced with height 1.

Main logic to understand:
Rotations locally rearrange three nodes while keeping the BST order,
which restores balance without rebuilding the whole tree.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int height(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int getBalance(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insert(Node* node, int value) {
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->data) {
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        node->right = insert(node->right, value);
    } else {
        return node;
    }

    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    if (balance > 1 && value < node->left->data) {
        return rightRotate(node);
    }
    if (balance < -1 && value > node->right->data) {
        return leftRotate(node);
    }
    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main() {
    Node* root = NULL;
    int n, value;

    printf("AVL Tree\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }

    printf("Inorder traversal (sorted): ");
    inorder(root);
    printf("\n");
    printf("Height of the AVL tree: %d\n", height(root));

    return 0;
}
