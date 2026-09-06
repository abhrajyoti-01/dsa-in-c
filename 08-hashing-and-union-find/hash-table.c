/*
Hash Table with Chaining
------------------------
What this program solves:
Menu-driven hash table using division hashing and separate
chaining for collision resolution: insert, search, delete, display
load factor.

Real life example:
A library index: call the key (book number) through a formula to
jump straight to the correct shelf bucket - no scanning. If two
books map to the same shelf, they hang on the same chain in that
shelf (chaining).

DIAGRAM (chaining: collisions share a bucket's chain):
  hash(key) = key % 5
  bucket 2: -> [12] -> [22] -> [32] -> NULL
  bucket 4: -> [ 4] -> NULL
  insert at chain head = O(1); search walks only its bucket.

How the process works:
1. hash(key) = key % TABLE_SIZE picks the bucket.
2. Each bucket is a linked list; new items insert at the bucket
   head (O(1)).
3. Collisions: different keys landing in the same bucket coexist on
   the chain.
4. Search/delete walk only that bucket's chain - expected O(1 +
   load factor), worst O(n) if everything collides.

Pseudo code:
START
INSERT(k, v): idx = k % SIZE; node(k, v).next = bucket[idx]
    bucket[idx] = node
SEARCH(k): idx = k % SIZE; walk bucket[idx] for key k
DELETE(k): find node in bucket[idx]; unlink; free
END

Sample input and dry run (SIZE 10):
insert 12, 22 -> both hash to bucket 2 -> chain 22 -> 12
search 22: found at bucket 2, position 1 on chain
delete 12: chain becomes just 22

Main logic to understand:
A hash table trades memory for O(1) average operations. The load
factor (n / table size) predicts chain length: keep it < 1 for
speed. Chaining never fills up; open addressing would need
rehashing.
*/

#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct HashNode {
    int key;
    int value;
    struct HashNode* next;
} HashNode;

HashNode* table[TABLE_SIZE];

/*
 * hashFunction - Division method: key mod table size.
 */
int hashFunction(int key) {
    return key % TABLE_SIZE;
}

/*
 * insertPair - O(1) insertion at the bucket head.
 */
void insertPair(int key, int value) {
    int idx = hashFunction(key);

    HashNode* curr = table[idx];
    while (curr != NULL) {
        if (curr->key == key) {
            curr->value = value;
            printf("Key %d updated to %d\n", key, value);
            return;
        }
        curr = curr->next;
    }

    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    node->key = key;
    node->value = value;
    node->next = table[idx];
    table[idx] = node;
    printf("Inserted (%d -> %d) in bucket %d\n", key, value, idx);
}

/*
 * searchKey - Walks only the one bucket chain.
 */
void searchKey(int key) {
    int idx = hashFunction(key);
    HashNode* curr = table[idx];
    int pos = 1;
    while (curr != NULL) {
        if (curr->key == key) {
            printf("Key %d -> value %d (bucket %d, chain pos %d)\n",
                   key, curr->value, idx, pos);
            return;
        }
        curr = curr->next;
        pos = pos + 1;
    }
    printf("Key %d not found\n", key);
}

/*
 * deleteKey - Unlinks the node from its bucket chain.
 */
void deleteKey(int key) {
    int idx = hashFunction(key);
    HashNode* curr = table[idx];
    HashNode* prev = NULL;

    while (curr != NULL) {
        if (curr->key == key) {
            if (prev == NULL) {
                table[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Deleted key %d\n", key);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Key %d not found\n", key);
}

/*
 * displayTable - Shows every bucket and its chain.
 */
void displayTable(void) {
    int items = 0;
    printf("\nHash table (buckets %d):\n", TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("  [%d]: ", i);
        for (HashNode* curr = table[i]; curr != NULL; curr = curr->next) {
            printf("(%d->%d) ", curr->key, curr->value);
            items = items + 1;
        }
        printf("\n");
    }
    printf("Items: %d, load factor: %.2f\n", items, (double)items / TABLE_SIZE);
}

int main() {
    int choice;
    int key;
    int value;

    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }

    while (1) {
        printf("\nHash Table (Separate Chaining, size %d)\n", TABLE_SIZE);
        printf("1. Insert key-value pair\n");
        printf("2. Search key\n");
        printf("3. Delete key\n");
        printf("4. Display table\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter key and value: ");
            scanf("%d %d", &key, &value);
            insertPair(key, value);
        } else if (choice == 2) {
            printf("Enter key: ");
            scanf("%d", &key);
            searchKey(key);
        } else if (choice == 3) {
            printf("Enter key: ");
            scanf("%d", &key);
            deleteKey(key);
        } else if (choice == 4) {
            displayTable();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    /* STEP: free all chains before exit */
    for (int i = 0; i < TABLE_SIZE; i++) {
        while (table[i] != NULL) {
            HashNode* next = table[i]->next;
            free(table[i]);
            table[i] = next;
        }
    }

    return 0;
}
