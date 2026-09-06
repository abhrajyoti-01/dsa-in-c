/*
Expression Tree
---------------
What this program solves:
This program converts an infix arithmetic expression into postfix form,
builds an expression tree from it, evaluates the tree, and prints the
expression in prefix, infix and postfix notation using traversals.

Real life example:
Every calculator and compiler does this internally. When you type
3 + 4 * 2, the parser must respect precedence (multiply first). It
converts the input to postfix "3 4 2 * +", builds a tree where leaves
are numbers and inner nodes are operators, then walks the tree to get
the answer 11.

DIAGRAM (postfix "a b + c *" builds this tree):
        *
       / \
      +   c
     / \
    a   b
  in-order:  a + b * c        post-order: a b + c *

How the process works:
1. Infix to postfix (shunting yard): operands go straight to output;
   operators wait on a stack and pop when a higher/equal precedence
   operator arrives or a ')' is met.
2. Build tree from postfix: push operands as leaf nodes; for each
   operator pop two nodes (right then left), attach them as children
   and push the new subtree.
3. Evaluate: recursively compute left and right, then apply the operator.
4. Traversals: inorder gives infix, preorder gives prefix, postorder
   gives postfix back.

Pseudo code:
START
InfixToPostfix(expression)
    For each symbol
        operand: append to output
        operator: pop stack while precedence >= current, then push
        '(': push;  ')': pop until '('
    Pop remaining stack to output

BuildTree(postfix)
    For each symbol
        operand: push leaf node
        operator: pop right, pop left, push new operator node
END

Sample input and dry run:
Infix: (3+4)*2
Postfix: 3 4 + 2 *
Tree building: push 3, push 4, '+' pops them into +(3,4); push 2;
'*' pops +(3,4) and 2 into *(+(3,4), 2).
Evaluation: 3+4 = 7, then 7*2 = 14.

Main logic to understand:
The tree shape itself encodes precedence and parentheses - evaluation
order is simply "children before parent", which is postorder traversal.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(char value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void infixToPostfix(const char* infix, char* postfix) {
    char stack[MAX];
    int top = -1;
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];

        if (isalnum(c)) {
            postfix[j++] = c;
        } else if (c == '(') {
            stack[++top] = c;
        } else if (c == ')') {
            while (top >= 0 && stack[top] != '(') {
                postfix[j++] = stack[top--];
            }
            if (top >= 0) {
                top--;
            }
        } else {
            while (top >= 0 && precedence(stack[top]) >= precedence(c)) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = c;
        }
    }

    while (top >= 0) {
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
}

TreeNode* buildTree(const char* postfix) {
    TreeNode* stack[MAX];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isalnum(c)) {
            stack[++top] = createNode(c);
        } else {
            TreeNode* node = createNode(c);
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        }
    }

    return stack[top];
}

int evaluate(TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return root->data - '0';
    }

    int leftVal = evaluate(root->left);
    int rightVal = evaluate(root->right);

    switch (root->data) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return rightVal != 0 ? leftVal / rightVal : 0;
    }
    return 0;
}

void inorder(TreeNode* root) {
    if (root != NULL) {
        if (root->left || root->right) printf("(");
        inorder(root->left);
        printf("%c", root->data);
        inorder(root->right);
        if (root->left || root->right) printf(")");
    }
}

void preorder(TreeNode* root) {
    if (root != NULL) {
        printf("%c ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(TreeNode* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%c ", root->data);
    }
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Expression Tree\n");
    printf("Enter an infix expression (single digits, e.g. (3+4)*2): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);
    printf("Postfix form: %s\n", postfix);

    TreeNode* root = buildTree(postfix);

    printf("Inorder  (infix):  ");
    inorder(root);
    printf("\n");

    printf("Preorder (prefix): ");
    preorder(root);
    printf("\n");

    printf("Postorder(postfix): ");
    postorder(root);
    printf("\n");

    printf("Result of evaluation: %d\n", evaluate(root));

    return 0;
}
