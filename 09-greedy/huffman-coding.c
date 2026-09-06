/*
 PROBLEM DEFINITION:
 --------------------
 Huffman Coding is a lossless data compression algorithm that assigns
 variable-length binary codes to characters based on their frequencies:
 - Frequently used characters get SHORT codes.
 - Rarely used characters get LONGER codes.
 It builds a binary tree (Huffman Tree) from the bottom up by repeatedly
 merging the two least frequent nodes. The code for each character is the
 path from root to its leaf (left = 0, right = 1).

 KEY PROPERTY: It is a PREFIX-FREE code - no character's code is a prefix
 of another's, so encoded text can be decoded unambiguously without separators.

 REAL-LIFE EXAMPLE - ZIP Files & JPEG Images:
 ---------------------------------------------
 When you compress a file with ZIP or save a JPEG image, Huffman coding
 (or a variant) is often used inside the pipeline. Consider an English
 text document: the letter 'e' appears very often, while 'z' appears rarely.
 Instead of spending 8 bits (ASCII) on every character, Huffman gives 'e'
 maybe 3 bits and 'z' maybe 10 bits. Frequent letters "pay less", so the
 total file size shrinks dramatically. Radio broadcasting (Morse code idea)
 and Bluetooth also use similar frequency-based prefix coding.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of it as repeatedly matchmaking the two "weakest" players until
 one tournament champion tree remains:

 1. COUNT FREQUENCIES:
    - Scan the input and count how many times each character appears.

 2. CREATE LEAF NODES:
    - Every unique character becomes a leaf node holding its frequency.

 3. BUILD THE TREE (Greedy merge):
    - Put all nodes in a priority queue (min-heap by frequency).
    - Repeat until one node remains:
        a) Extract the two nodes with the SMALLEST frequencies.
        b) Create a new internal node with these two as children.
        c) Its frequency = sum of the two children's frequencies.
        d) Insert the new node back into the queue.
    - The last remaining node is the ROOT of the Huffman Tree.

 4. ASSIGN CODES:
    - Traverse the tree: going LEFT appends '0', going RIGHT appends '1'.
    - The path from root to each leaf is that character's code.

 5. WHY GREEDY WORKS:
    - Merging the smallest frequencies first buries rare characters
      deepest (longest codes) and keeps frequent ones near the root
      (shortest codes) - proven optimal for prefix-free codes.

 PSEUDO CODE:
 ------------
 START
   Function BuildHuffman(freq[]):
     Create a leaf node for each character, insert into min-heap Q
     While Q has more than one node:
         left  = ExtractMin(Q)
         right = ExtractMin(Q)
         internal = new Node('$')
         internal.freq = left.freq + right.freq
         internal.left = left
         internal.right = right
         Insert internal into Q
     Return ExtractMin(Q)      // the root

   Function AssignCodes(node, code):
     If node is a leaf:
         Store code for node.character
         Return
     AssignCodes(node.left,  code + '0')
     AssignCodes(node.right, code + '1')

   Function Encode(text):
     For each character c in text: Output code[c]
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Text characters and frequencies:
   a: 5,  b: 9,  c: 12,  d: 13,  e: 16,  f: 45

 Building the tree (merge two smallest each time):

 Step 1: Extract a(5), b(9) -> merge into N1(14)
         Queue: {N1(14), c(12), d(13), e(16), f(45)}
 Step 2: Extract c(12), d(13) -> merge into N2(25)
         Queue: {N1(14), N2(25), e(16), f(45)}
 Step 3: Extract N1(14), e(16) -> merge into N3(30)
         Queue: {N2(25), N3(30), f(45)}
 Step 4: Extract N2(25), N3(30) -> merge into N4(55)
         Queue: {f(45), N4(55)}
 Step 5: Extract f(45), N4(55) -> merge into ROOT(100)
         Queue: {ROOT} - done!

 Tree shape:
                 (100)
                0/   \1
              f(45)  (55)
                    0/   \1
                  (25)   (30)
                 0/  \1  0/  \1
               c(12) d(13) (14) e(16)
                           0/\1
                         a(5) b(9)

 READ THE DIAGRAM: every left turn = '0', every right turn = '1'.
 f is the most frequent (45) and sits at depth 1 -> code "0" (1 bit).
 a is rare (5) and sits deepest -> code "1100" (4 bits).
 Frequent characters "pay less" - that IS the compression.

 Final codes (shorter for frequent chars):
   f: 0           (freq 45 - 1 bit!)
   c: 100         d: 101
   e: 111         a: 1100      b: 1101

 Encoding cost comparison:
   Fixed 3-bit code:  (5+9+12+13+16+45) x 3 = 300 bits
   Huffman code:      5x4 + 9x4 + 12x3 + 13x3 + 16x3 + 45x1 = 224 bits
   Compression saved: 76 bits (~25%)

 TIME COMPLEXITY:
 - Building tree: O(N log N) where N = number of unique characters
 - Encoding text: O(total characters x average code length)

 SPACE COMPLEXITY: O(N) for the tree and code table

 COMMON USE CASES:
 - File compression (ZIP, GZIP internally)
 - Image formats (JPEG entropy coding stage)
 - MP3 audio compression
 - Network protocols (HTTP/2 HPACK header compression)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256

/*
 * Node Structure:
 * Represents one node in the Huffman Tree.
 * Leaf nodes hold an actual character; internal nodes hold '$' as a marker.
 */
typedef struct Node {
    char character;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;

/*
 * createNode - Factory for a new Huffman tree node.
 */
Node* createNode(char character, int frequency) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*
 * Simple priority queue implemented as a plain array with linear-scan
 * extraction of the minimum. For learning clarity; a binary heap gives
 * O(log N) extraction (see heap_implementation.c in this collection).
 */
typedef struct {
    Node* items[MAX_CHARS];
    int size;
} PriorityQueue;

/*
 * initQueue - Resets the priority queue.
 */
void initQueue(PriorityQueue* pq) {
    pq->size = 0;
}

/*
 * insert - Adds a node to the priority queue.
 */
void insert(PriorityQueue* pq, Node* node) {
    pq->items[pq->size++] = node;
}

/*
 * extractMin - Removes and returns the node with smallest frequency.
 * Analogy: The tournament picks the two weakest players to merge first.
 */
Node* extractMin(PriorityQueue* pq) {
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->items[i]->frequency < pq->items[minIndex]->frequency) {
            minIndex = i;
        }
    }
    Node* minNode = pq->items[minIndex];
    pq->items[minIndex] = pq->items[pq->size - 1];
    pq->size--;
    return minNode;
}

/*
 * codeTable - Stores the generated binary code string for each character.
 * Indexed by the character's ASCII value. This lets encoding be a simple
 * table lookup instead of re-searching the tree for every character.
 */
char codeTable[MAX_CHARS][64];

/*
 * buildCodes - Recursively traverses the tree, building each leaf's code.
 * Going left appends '0'; going right appends '1'.
 *
 * Analogy: Every turn in a corridor adds one digit to the room number.
 */
void buildCodes(Node* root, int code[], int top) {
    if (root->left == NULL && root->right == NULL) {
        code[top] = '\0';

        for (int i = 0; i < top; i++) {
            codeTable[(unsigned char)root->character][i] = (char)('0' + code[i]);
        }
        if (top == 0) {
            codeTable[(unsigned char)root->character][0] = '0';
            codeTable[(unsigned char)root->character][1] = '\0';
        } else {
            codeTable[(unsigned char)root->character][top] = '\0';
        }
        return;
    }

    if (root->left != NULL) {
        code[top] = 0;
        buildCodes(root->left, code, top + 1);
    }

    if (root->right != NULL) {
        code[top] = 1;
        buildCodes(root->right, code, top + 1);
    }
}

/*
 * printCodes - Displays each character alongside its generated code.
 */
void printCodes(Node* root, int code[], int top) {
    if (root->left == NULL && root->right == NULL) {
        printf("  '%c' (freq %2d) : %-8s  (%d bit%s)\n",
               root->character, root->frequency,
               codeTable[(unsigned char)root->character],
               (int)strlen(codeTable[(unsigned char)root->character]),
               (strlen(codeTable[(unsigned char)root->character]) <= 1) ? "" : "s");
        return;
    }

    if (root->left != NULL) {
        code[top] = 0;
        printCodes(root->left, code, top + 1);
    }

    if (root->right != NULL) {
        code[top] = 1;
        printCodes(root->right, code, top + 1);
    }
}

/*
 * encodeText - Concatenates the stored codes for every character in text.
 * Because the codes are prefix-free, the bit stream decodes unambiguously.
 */
void encodeText(const char* text) {
    printf("\n  Encoded bits for \"%s\":\n  ", text);

    int totalBits = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        const char* code = codeTable[(unsigned char)text[i]];
        printf("%s ", code);
        totalBits += (int)strlen(code);
    }

    printf("\n  Total = %d bits\n", totalBits);
}

/*
 * freeTree - Releases all memory used by the Huffman tree.
 */
void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/*
 * collectCodeLengths - Records each leaf character's code length.
 */
void collectCodeLengths(Node* root, int code[], int depth, int codeLen[]) {
    if (root == NULL) {
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        codeLen[(unsigned char)root->character] = depth;
        return;
    }
    collectCodeLengths(root->left, code, depth + 1, codeLen);
    collectCodeLengths(root->right, code, depth + 1, codeLen);
}

int main() {
    char characters[64];
    int frequencies[64];
    int n;

    printf("Huffman Coding - Greedy Data Compression\n\n");

    printf("How many distinct characters (1-64)? ");
    scanf("%d", &n);

    if (n < 1 || n > 64) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d characters with no spaces between them (e.g. abcdef): ", n);
    scanf("%63s", characters);

    if ((int)strlen(characters) != n) {
        printf("Expected %d characters, got %d\n", n, (int)strlen(characters));
        return 1;
    }

    printf("Enter %d frequencies separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &frequencies[i]);
    }

    printf("\nInput characters and frequencies:\n");
    for (int i = 0; i < n; i++) {
        printf("  '%c' : %d\n", characters[i], frequencies[i]);
    }
    printf("\n");

    PriorityQueue pq;
    initQueue(&pq);

    for (int i = 0; i < n; i++) {
        insert(&pq, createNode(characters[i], frequencies[i]));
    }

    /* STEP: Greedily merge the two smallest-frequency nodes until one tree remains */
    int mergeStep = 1;
    while (pq.size > 1) {
        Node* left = extractMin(&pq);
        Node* right = extractMin(&pq);

        Node* internal = createNode('$', left->frequency + right->frequency);
        internal->left = left;
        internal->right = right;

        printf("  Step %d: merge %d + %d = %d\n",
               mergeStep++, left->frequency, right->frequency, internal->frequency);

        insert(&pq, internal);
    }

    Node* root = extractMin(&pq);
    printf("\n  Root frequency (total): %d\n", root->frequency);

    /* STEP: Assign prefix-free codes by traversing the tree (left = 0, right = 1) */
    int code[64];
    buildCodes(root, code, 0);
    printCodes(root, code, 0);

    /* STEP: Encode text typed by the user using the generated codes */
    char text[256];
    printf("\nEnter text to encode (characters used above): ");
    scanf("%255s", text);

    encodeText(text);

    /* STEP: Compare Huffman cost against fixed-length encoding */
    int codeLen[256];
    for (int i = 0; i < 256; i++) {
        codeLen[i] = 0;
    }
    collectCodeLengths(root, code, 0, codeLen);

    int totalFreq = 0;
    int bitsHuffman = 0;
    int maxLen = 0;
    for (int i = 0; i < n; i++) {
        totalFreq += frequencies[i];
        bitsHuffman += frequencies[i] * codeLen[(unsigned char)characters[i]];
        if (codeLen[(unsigned char)characters[i]] > maxLen) {
            maxLen = codeLen[(unsigned char)characters[i]];
        }
    }

    int bitsPerChar = 1;
    while ((1 << bitsPerChar) < n) {
        bitsPerChar = bitsPerChar + 1;
    }

    int bitsFixed = totalFreq * bitsPerChar;

    printf("\n  Fixed-length (%d bits/char): %d bits\n", bitsPerChar, bitsFixed);
    printf("  Huffman coding:             %d bits\n", bitsHuffman);
    printf("  Compression saved:          %d bits (%.1f%%)\n",
           bitsFixed - bitsHuffman,
           (bitsFixed > 0) ? 100.0 * (bitsFixed - bitsHuffman) / bitsFixed : 0.0);

    freeTree(root);
    return 0;
}
