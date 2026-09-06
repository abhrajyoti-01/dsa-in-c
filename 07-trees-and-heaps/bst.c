/*
Binary Search Tree (BST)
------------------------
What this program solves:
This program builds a Binary Search Tree and supports insert, search,
delete and three traversals (inorder, preorder, postorder).

Real life example:
Think of a library catalog organized by call numbers. When a new book
arrives, the system compares its call number with the current shelf and
goes left for smaller numbers, right for bigger ones, until it finds an
empty spot. Searching works the same way, so lookup is very fast.

DIAGRAM (after inserting 50, 30, 70, 20, 40):
              50
             /  \
           30    70
          /  \
        20    40

Search 40: 50 -> 40 > 50? no, go LEFT -> 30 -> 40 > 30, go RIGHT
-> 40 FOUND (3 comparisons instead of scanning 5 nodes).

Delete 30 (two children): replace with inorder SUCCESSOR (40) -
the smallest value in the right subtree:
              50
             /  \
           40    70
          /
        20

How the process works:
1. Insert: compare the value with the current node, go left if smaller,
   right if bigger, and place it at the empty spot.
2. Search: follow the same comparisons until found or NULL is reached.
3. Inorder traversal (Left, Root, Right) prints values in sorted order.
4. Delete: a leaf is removed directly; a node with two children is
   replaced by its inorder successor (smallest value in right subtree).

Pseudo code:
START
Insert(root, value)
    If root is NULL create a new node
    If value < root data go left else go right
Search(root, target)
    If root is NULL or matches target return root
    If target < root data search left else search right
Inorder(root)
    Visit left, print root, visit right
END

Sample input and dry run:
Insert 50 30 70 20 40
50 becomes root, 30 goes left of 50, 70 goes right of 50,
20 goes left of 30, 40 goes right of 30
Inorder gives 20 30 40 50 70 (sorted order)

Main logic to understand:
The BST property (left smaller, right bigger) is what makes every
comparison eliminate half of the remaining tree, giving O(log n)
search on average.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    return root;
}

Node* search(Node* root, int target) {
    if (root == NULL || root->data == target) {
        return root;
    }
    if (target < root->data) {
        return search(root->left, target);
    }
    return search(root->right, target);
}

Node* findMin(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return root;
    }
    if (value < root->data) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->data) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* successor = findMin(root->right);
        root->data = successor->data;
        root->right = deleteNode(root->right, successor->data);
    }
    return root;
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main() {
    Node* root = NULL;
    int choice, value;

    while (1) {
        printf("\nBinary Search Tree\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Inorder traversal\n");
        printf("5. Preorder traversal\n");
        printf("6. Postorder traversal\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter value to insert: ");
            scanf("%d", &value);
            root = insert(root, value);
            printf("Inserted %d\n", value);
        } else if (choice == 2) {
            printf("Enter value to search: ");
            scanf("%d", &value);
            if (search(root, value) != NULL) {
                printf("%d found in the tree\n", value);
            } else {
                printf("%d not found\n", value);
            }
        } else if (choice == 3) {
            printf("Enter value to delete: ");
            scanf("%d", &value);
            root = deleteNode(root, value);
            printf("Deleted %d (if present)\n", value);
        } else if (choice == 4) {
            printf("Inorder: ");
            inorder(root);
            printf("\n");
        } else if (choice == 5) {
            printf("Preorder: ");
            preorder(root);
            printf("\n");
        } else if (choice == 6) {
            printf("Postorder: ");
            postorder(root);
            printf("\n");
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
