/*
Splay Tree
----------
What this program solves:
A self-adjusting binary search tree: every accessed node is rotated
to the ROOT with splay operations. Frequently used keys become
cheap to reach; the tree needs no height field (unlike AVL).

Real life example:
A vending machine that reshelves your favourite snack at the front
each time you buy it: recently bought items are always near the
entrance. Same idea - recently accessed data moves to the root.

DIAGRAM (accessing 10 splays it to the root):
      30                     10
     /  \        zig-zig   /    \
    20   40    ------->  ...     30
   /                            /
  10                          20
  recently touched keys become the cheapest to touch again.

How the process works:
1. BST insert/search as usual, then SPLAY the node:
   - Zig: one rotation when parent is the root.
   - Zig-zig: rotate grandparent-parent, then parent-node.
   - Zig-zag: rotate parent-node, then parent-node again.
2. Each splay halves the distance to the root, so repeated access
   of the same key costs O(1) amortized.
3. Delete: splay the victim, remove it, join left and right
   subtrees (splay the max of the left tree, attach right).

Pseudo code:
START
SPLAY(x): WHILE x has grandparent
    zig-zig: rotate(g, p); rotate(p, x)
    zig-zag: rotate(p, x); rotate(g, x)
    one more rotate if parent is root
INSERT(k): BST insert; splay(new node)
SEARCH(k): BST search; splay(found node or last visited)
END

Sample input and dry run:
insert 10, 20, 30 (sequential - worst case for plain BST)
each splay rebalances: no degenerate chain forms
search 10 twice -> second search finds it at the root immediately

Main logic to understand:
No balance metadata is stored; balance emerges from splashing
recent nodes upward. Amortized O(log n) per operation, O(1) worst
for the just-accessed node. Great for caches; poor for uniform
random access (more rotations than AVL).
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct SNode {
    int key;
    struct SNode* left;
    struct SNode* right;
} SNode;

/*
 * newSNode - Allocates a node.
 */
SNode* newSNode(int key) {
    SNode* node = (SNode*)malloc(sizeof(SNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
 * rotateRight - Brings the left child up.
 */
SNode* rotateRight(SNode* x) {
    SNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

/*
 * rotateLeft - Brings the right child up.
 */
SNode* rotateLeft(SNode* x) {
    SNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

/*
 * splay - Moves the node with given key to the root.
 */
SNode* splay(SNode* root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (key < root->key) {
        if (root->left == NULL) {
            return root;
        }
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL) {
                root->left = rotateLeft(root->left);
            }
        }
        return (root->left == NULL) ? root : rotateRight(root);
    } else {
        if (root->right == NULL) {
            return root;
        }
        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        } else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL) {
                root->right = rotateRight(root->right);
            }
        }
        return (root->right == NULL) ? root : rotateLeft(root);
    }
}

/*
 * splayInsert - BST insert followed by splaying to the root.
 */
SNode* splayInsert(SNode* root, int key) {
    if (root == NULL) {
        printf("Inserted %d\n", key);
        return newSNode(key);
    }
    root = splay(root, key);
    if (root->key == key) {
        printf("%d already exists\n", key);
        return root;
    }

    SNode* node = newSNode(key);
    if (key < root->key) {
        node->right = root;
        node->left = root->left;
        root->left = NULL;
    } else {
        node->left = root;
        node->right = root->right;
        root->right = NULL;
    }
    printf("Inserted %d (splayed to root)\n", key);
    return node;
}

/*
 * splaySearch - Searches and splays the found (or closest) node.
 */
SNode* splaySearch(SNode* root, int key) {
    root = splay(root, key);
    if (root != NULL && root->key == key) {
        printf("Found %d - it is now the root\n", key);
    } else {
        printf("%d not found (nearest node splayed to root)\n", key);
    }
    return root;
}

/*
 * splayDelete - Removes key; joins the two remaining subtrees.
 */
SNode* splayDelete(SNode* root, int key) {
    if (root == NULL) {
        printf("%d not found\n", key);
        return NULL;
    }
    root = splay(root, key);
    if (root->key != key) {
        printf("%d not found\n", key);
        return root;
    }

    SNode* left = root->left;
    SNode* right = root->right;
    free(root);
    printf("Deleted %d\n", key);

    if (left == NULL) {
        return right;
    }
    SNode* maxLeft = left;
    while (maxLeft->right != NULL) {
        maxLeft = maxLeft->right;
    }
    left = splay(left, maxLeft->key);
    left->right = right;
    return left;
}

/*
 * preorder - Indented display showing tree shape.
 */
void preorder(SNode* root, int depth) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%d\n", root->key);
    preorder(root->left, depth + 1);
    preorder(root->right, depth + 1);
}

/*
 * freeTree - Releases all nodes.
 */
void freeTree(SNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    SNode* root = NULL;
    int choice;
    int key;

    printf("Splay Tree (Self-Adjusting BST)\n\n");

    while (1) {
        printf("\n1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Display tree (preorder, indented)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter key: ");
            scanf("%d", &key);
            root = splayInsert(root, key);
        } else if (choice == 2) {
            printf("Enter key: ");
            scanf("%d", &key);
            root = splaySearch(root, key);
        } else if (choice == 3) {
            printf("Enter key: ");
            scanf("%d", &key);
            root = splayDelete(root, key);
        } else if (choice == 4) {
            if (root == NULL) {
                printf("Tree is empty\n");
            } else {
                printf("Root is %d (most recently accessed)\n", root->key);
                preorder(root, 0);
            }
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    freeTree(root);
    return 0;
}
