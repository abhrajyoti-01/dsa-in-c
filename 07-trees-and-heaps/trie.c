/*
Trie (Prefix Tree)
------------------
What this program solves:
This program stores a set of lowercase words in a trie and supports
insert, full-word search, prefix check and auto-complete listing of all
words with a given prefix.

Real life example:
When you type "app" on a phone keyboard, it instantly suggests "apple",
"application" and so on. The phone keeps all dictionary words in a trie:
words sharing a prefix share the same path of nodes, so finding every
word starting with "app" is just walking three nodes and collecting
everything below.

DIAGRAM (CAT, CAR, DOG share prefix nodes):
        root
       /    \
      c      d
      |      |
      a      o
     / \     |
    t*  r*   g*        * = end-of-word flag
  lookup walks ONE node per character; shared prefixes stored once.

How the process works:
1. Each node has 26 children (one per letter) and an end-of-word flag.
2. Insert: walk character by character, creating missing nodes, and
   mark the last node as end of word.
3. Search: walk the characters; the word exists only if the path exists
   AND the last node is marked as end of word.
4. Prefix search: same walk, but only the path needs to exist.
5. Auto-complete: walk to the prefix node, then list all end-of-word
   markers in its subtree.

Pseudo code:
START
Insert(word)
    node = root
    For each character c in word
        If child for c missing, create it
        Move to that child
    Mark node as end of word

Search(word)
    Walk the characters from root
    Return true only if path exists and last node is end of word
END

Sample input and dry run:
Insert cat, car, dog
c-a-t path created and t marked; c-a-r reuses c and a, adds r;
d-o-g created separately.
Search "cat" found; search "ca" not found (only a prefix);
prefix "ca" exists -> auto-complete lists cat and car.

Main logic to understand:
Shared prefixes share nodes, which saves memory and makes prefix
operations independent of how many words are stored.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

TrieNode* createNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    newNode->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void insert(TrieNode* root, const char* word) {
    TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

int search(TrieNode* root, const char* word) {
    TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

int startsWith(TrieNode* root, const char* prefix) {
    TrieNode* current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }
    return 1;
}

void listWords(TrieNode* node, char* buffer, int depth) {
    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = 'a' + i;
            listWords(node->children[i], buffer, depth + 1);
        }
    }
}

void autoComplete(TrieNode* root, const char* prefix) {
    TrieNode* current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (current->children[index] == NULL) {
            printf("No words with prefix \"%s\"\n", prefix);
            return;
        }
        current = current->children[index];
    }

    char buffer[100];
    strcpy(buffer, prefix);
    listWords(current, buffer, strlen(prefix));
}

int main() {
    TrieNode* root = createNode();
    int choice;
    char word[100];

    while (1) {
        printf("\nTrie (Prefix Tree)\n");
        printf("1. Insert word\n");
        printf("2. Search word\n");
        printf("3. Check prefix\n");
        printf("4. Auto-complete (list words with prefix)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter word (lowercase): ");
            scanf("%s", word);
            insert(root, word);
            printf("Inserted \"%s\"\n", word);
        } else if (choice == 2) {
            printf("Enter word to search: ");
            scanf("%s", word);
            if (search(root, word)) {
                printf("\"%s\" found\n", word);
            } else {
                printf("\"%s\" not found\n", word);
            }
        } else if (choice == 3) {
            printf("Enter prefix: ");
            scanf("%s", word);
            if (startsWith(root, word)) {
                printf("Words with prefix \"%s\" exist\n", word);
            } else {
                printf("No words with prefix \"%s\"\n", word);
            }
        } else if (choice == 4) {
            printf("Enter prefix: ");
            scanf("%s", word);
            printf("Suggestions:\n");
            autoComplete(root, word);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
