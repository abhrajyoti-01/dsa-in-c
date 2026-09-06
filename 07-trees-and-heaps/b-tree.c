/*
B-Tree
------
What this program solves:
This program implements a B-Tree of minimum degree 2 that supports
insertion (with node splitting) and search.

Real life example:
Databases like MySQL store their indexes in B-Trees. Disk reads are
slow, so instead of a binary tree with many levels, a B-Tree packs many
keys into each node (one disk block). One million records need only
about 3 levels, so a lookup costs 3 disk reads instead of 20.

DIAGRAM (one node = one disk block holding several keys):
         [ 30 | 60 ]
        /     |     \
  [10 20] [40 50] [70 80]
  insert into a FULL child: split it, the middle key moves UP;
  a full root splits first and the tree grows one level.
  height ~ log_t(n): 3 levels can index millions of keys.

How the process works:
1. Each node holds between 1 and 3 keys (for minimum degree t = 2).
2. Insert like a BST, but before entering a FULL child (3 keys), split
   it: the middle key moves up to the parent and the node divides in two.
3. If the root itself is full, split it first, growing the tree upward.
4. Search scans the keys of a node, then descends into the child whose
   range contains the target.

Pseudo code:
START
Insert(root, key)
    If root is full: create new root, split old root as its child
    InsertNonFull(root, key)

InsertNonFull(node, key)
    If node is leaf: place key in sorted position
    Else:
        Find child index i for key
        If that child is full: split it, adjust i
        InsertNonFull(child i, key)
END

Sample input and dry run:
Insert 10, 20, 5, 6
After 10, 20, 5 the root holds [5, 10, 20] and is full.
Inserting 6 splits the root: 10 moves up, left child [5], right [20],
then 6 goes into the left child -> [5, 6].
Final tree: root [10] with children [5,6] and [20].

Main logic to understand:
Splitting full nodes on the way DOWN guarantees every node we enter has
room, so insertion never needs to backtrack - one pass, always balanced.
*/
#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct BTreeNode {
    int keys[2 * T - 1];
    struct BTreeNode* children[2 * T];
    int count;
    int isLeaf;
} BTreeNode;

BTreeNode* createNode(int isLeaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->count = 0;
    node->isLeaf = isLeaf;
    for (int i = 0; i < 2 * T; i++) {
        node->children[i] = NULL;
    }
    return node;
}

BTreeNode* search(BTreeNode* root, int key) {
    int i = 0;
    while (i < root->count && key > root->keys[i]) {
        i++;
    }
    if (i < root->count && root->keys[i] == key) {
        return root;
    }
    if (root->isLeaf) {
        return NULL;
    }
    return search(root->children[i], key);
}

void splitChild(BTreeNode* parent, int index, BTreeNode* child) {
    BTreeNode* sibling = createNode(child->isLeaf);
    sibling->count = T - 1;

    for (int j = 0; j < T - 1; j++) {
        sibling->keys[j] = child->keys[j + T];
    }
    if (!child->isLeaf) {
        for (int j = 0; j < T; j++) {
            sibling->children[j] = child->children[j + T];
        }
    }
    child->count = T - 1;

    for (int j = parent->count; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = sibling;

    for (int j = parent->count - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[index] = child->keys[T - 1];

    parent->count++;
}

void insertNonFull(BTreeNode* node, int key) {
    int i = node->count - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->count++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->count == 2 * T - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

BTreeNode* insert(BTreeNode* root, int key) {
    if (root->count == 2 * T - 1) {
        BTreeNode* newRoot = createNode(0);
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);

        int i = 0;
        if (key > newRoot->keys[0]) {
            i = 1;
        }
        insertNonFull(newRoot->children[i], key);
        return newRoot;
    }
    insertNonFull(root, key);
    return root;
}

void traverse(BTreeNode* root) {
    int i;
    for (i = 0; i < root->count; i++) {
        if (!root->isLeaf) {
            traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }
    if (!root->isLeaf) {
        traverse(root->children[i]);
    }
}

int main() {
    BTreeNode* root = createNode(1);
    int n, value;

    printf("B-Tree (minimum degree %d)\n", T);
    printf("Enter the number of keys: ");
    scanf("%d", &n);

    printf("Enter the keys:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }

    printf("Inorder traversal (sorted): ");
    traverse(root);
    printf("\n");

    printf("Enter a key to search: ");
    scanf("%d", &value);
    if (search(root, value) != NULL) {
        printf("%d found in the B-Tree\n", value);
    } else {
        printf("%d not found\n", value);
    }

    return 0;
}
