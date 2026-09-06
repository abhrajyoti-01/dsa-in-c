/*
Binary Tree - Complete Operations Ladder (Basic -> Hard)
---------------------------------------------------------
Real life example:
File systems, org charts and the HTML DOM are trees; the traversal
orders here are how parsers, printers and "find all files" utilities
walk them.

DIAGRAM (traversal orders of the same tree):
        1
       / \
      2   3         pre : 1 2 4 5 3     (node first)
     / \            in  : 4 2 5 1 3     (node middle)
    4   5           post: 4 5 2 3 1     (node last)
                    level: 1 2 3 4 5    (queue, row by row)

LADDER (easy -> hard inside this file):
  1. Build tree from user input    (nodes with left/right children)
  2. In/Pre/Post-order traversals  (recursive)
  3. Level-order traversal         (BFS with queue)
  4. Height of the tree            (max depth of subtrees + 1)
  5. Count nodes and leaves
  6. Mirror the tree               (in-place)
  7. Diameter                      (longest path between two nodes)

BEGINNER EXPLANATION:
A binary tree: each node has AT MOST two children (left, right).
Traversals differ in WHEN you visit the node:
- Pre-order: node -> left -> right   (copy a tree, prefix exprs)
- In-order: left -> node -> right    (sorted output for BSTs!)
- Post-order: left -> right -> node  (delete tree, postfix exprs)
- Level-order: row by row (needs a QUEUE, not recursion).

SUPER SIMPLE EXAMPLE:
      1
     / \
    2   3
In-order: 2 1 3 | Pre: 1 2 3 | Post: 2 3 1 | Level: 1 2 3

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Empty tree: height = 0 (or -1 by another convention - pick and
  document), all traversals print nothing.
- Single node: every traversal = that node; diameter = 0 edges.
- Skewed tree (all left children): height = n-1; in-order prints
  REVERSED sorted data; recursion depth = n (stack risk for large n).
- Diameter vs height: height counts EDGES on deepest path;
  diameter may NOT pass through the root - e.g. two long branches
  under a short root. That is why diameter = max over every node
  of (leftHeight + rightHeight), not just at the root.
- Mirror twice = original tree (self-check).

DRY RUN (tree 1(2,3), diameter):
node 2: leaves, heights 0,0 -> local diameter 0
node 1: left height 1, right height 1 -> local 1+1 = 2? With edge
counting: leftH=1, rightH=1 -> local = 2 edges through node 1.
Height of root = 2? No: height(leaf)=0, height(1) = 1+max(0,0)=1.
Hmm - definitions: this file counts NODES on the path; diameter
for the sample = 3 nodes (2-1-3). Numbers vary by convention -
the code documents its own convention clearly.

TIPS:
- In-order of a BST gives SORTED output - the fact behind
  validating BSTs (bst_problems next) and expression trees.
- Level-order = BFS = the queue discipline; depth-first = the
  recursion/stack family. Same tree, two philosophies.
- Diameter via "height that also records max" in ONE traversal
  is the elegant O(n) solution - try implementing after running.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct TNode {
    int data;
    struct TNode* left;
    struct TNode* right;
} TNode;

/*
 * newTNode - Allocates a tree node.
 */
TNode* newTNode(int value) {
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
 * buildTree - Asks for children level by level (0 = no child).
 */
TNode* buildTree(void) {
    int v;
    printf("Root value (0 for empty tree): ");
    scanf("%d", &v);
    if (v == 0) {
        return NULL;
    }
    TNode* root = newTNode(v);

    TNode* queue[200];
    int front = 0;
    int rear = 0;
    queue[rear] = root;
    rear = rear + 1;

    while (front < rear) {
        TNode* curr = queue[front];
        front = front + 1;

        printf("Left child of %d (0 = none): ", curr->data);
        scanf("%d", &v);
        if (v != 0) {
            curr->left = newTNode(v);
            queue[rear] = curr->left;
            rear = rear + 1;
        }
        printf("Right child of %d (0 = none): ", curr->data);
        scanf("%d", &v);
        if (v != 0) {
            curr->right = newTNode(v);
            queue[rear] = curr->right;
            rear = rear + 1;
        }
    }
    return root;
}

void preorder(TNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(TNode* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void postorder(TNode* root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

/*
 * levelOrder - BFS with an explicit queue.
 */
void levelOrder(TNode* root) {
    if (root == NULL) {
        return;
    }
    TNode* queue[200];
    int front = 0;
    int rear = 0;
    queue[rear] = root;
    rear = rear + 1;

    while (front < rear) {
        TNode* curr = queue[front];
        front = front + 1;
        printf("%d ", curr->data);
        if (curr->left != NULL) {
            queue[rear] = curr->left;
            rear = rear + 1;
        }
        if (curr->right != NULL) {
            queue[rear] = curr->right;
            rear = rear + 1;
        }
    }
}

/*
 * treeHeight - Height in NODES (empty = 0).
 */
int treeHeight(TNode* root) {
    if (root == NULL) {
        return 0;
    }
    int lh = treeHeight(root->left);
    int rh = treeHeight(root->right);
    return 1 + (lh > rh ? lh : rh);
}

/*
 * countNodes / countLeaves - Basic aggregations.
 */
int countNodes(TNode* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(TNode* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    return countLeaves(root->left) + countLeaves(root->right);
}

/*
 * mirrorTree - Swaps children of EVERY node in place.
 */
void mirrorTree(TNode* root) {
    if (root == NULL) {
        return;
    }
    TNode* t = root->left;
    root->left = root->right;
    root->right = t;
    mirrorTree(root->left);
    mirrorTree(root->right);
}

/*
 * diameterInfo - One-pass: returns height, records max diameter.
 */
int diameterInfo(TNode* root, int* best) {
    if (root == NULL) {
        return 0;
    }
    int lh = diameterInfo(root->left, best);
    int rh = diameterInfo(root->right, best);
    int local = lh + rh + 1;
    if (local > *best) {
        *best = local;
    }
    return 1 + (lh > rh ? lh : rh);
}

void freeTree(TNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    printf("Binary Tree Operations Ladder (easy -> hard)\n\n");

    TNode* root = buildTree();

    if (root == NULL) {
        printf("Empty tree - nothing to do\n");
        return 0;
    }

    printf("\nPre-order:   ");
    preorder(root);
    printf("\nIn-order:    ");
    inorder(root);
    printf("\nPost-order:  ");
    postorder(root);
    printf("\nLevel-order: ");
    levelOrder(root);
    printf("\n");

    printf("Height (nodes): %d\n", treeHeight(root));
    printf("Total nodes: %d, Leaves: %d\n",
           countNodes(root), countLeaves(root));

    int best = 0;
    diameterInfo(root, &best);
    printf("Diameter (nodes on longest path): %d\n", best);

    mirrorTree(root);
    printf("\nAfter mirror, in-order: ");
    inorder(root);
    printf("\n");

    freeTree(root);
    return 0;
}
