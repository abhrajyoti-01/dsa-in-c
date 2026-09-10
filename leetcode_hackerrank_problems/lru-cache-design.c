/*
LRU Cache Design - Doubly Linked List plus Hash Map (Intermediate -> Advanced)
-----------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 146 - LRU Cache (the canonical "design" interview question)
  LeetCode 460 - LFU Cache (the harder frequency-based follow-up)
  LeetCode 706 - Design HashMap (the table shown here)
  LeetCode 707 - Design Linked List (the list shown here)
  HackerRank "Cycle Detection" style design drills with pointer surgery

PROBLEM DEFINITION:
Implement a fixed-capacity cache whose operations are all O(1):
get(key) returns a value and marks it most recently used; put(key,
value) inserts or updates and evicts the LEAST recently used entry when
full. The hash map finds a node in O(1); a doubly linked list reorders
nodes in O(1).

REAL-LIFE EXAMPLE:
Database page caches, CDN edge caches and CPU memory hierarchies all
evict by recency. The "recently used" ordering must be maintainable
without scanning - hence the list plus map.

DIAGRAM (capacity 3, operations put 1:10, put 2:20, put 3:30, get 1,
put 4:40):
  MRU -> [3:30] [2:20] [1:10] <- LRU
  get 1 refreshes 1:          MRU -> [1:10] [3:30] [2:20] <- LRU
  put 4 evicts the LRU = 2:20
  MRU -> [4:40] [1:10] [3:30] <- LRU

  node arrays (no malloc):
    key[] value[] prev[] next[]     doubly linked list
    table[] maps key -> node index  open addressing, O(1) average

PSEUDO CODE:
  detach(node): prev[node] <-> next[node]  (skip the node)
  pushFront(node): insert right after the sentinel head
  moveToFront(node): detach then pushFront
  get(key): index = table[key]
      absent -> report -1 (miss)
      present -> moveToFront(index); return value
  put(key, value):
      present -> update value, moveToFront
      absent  -> if size == capacity: remove the node before the tail
                 sentinel (the LRU) and erase its key from the table
                 create a node, pushFront, insert into the table

DELETION IN AN OPEN-ADDRESSED TABLE (the trap this file demonstrates):
  Clearing a slot on erase is WRONG. If A and B collide and B was
  placed after A, then erasing A leaves a hole that stops the probe for
  B, so B becomes unreachable. Correct deletion leaves a TOMBSTONE:
  probes keep walking through it, and inserts may reuse the slot.
  A buggy erase routine is kept below so the built-in self-test can
  prove the difference at run time.

NODE REUSE (the second trap):
  Nodes must be recycled. Allocating a fresh node per put overflows the
  fixed arrays after capacity + evictions insertions - with 1000
  operations a capacity-1 cache would need 1000 nodes. This file keeps
  a free list of evicted node indices and pops one per insertion, so
  the footprint never exceeds `capacity` nodes.

SAMPLE INPUT & DRY RUN:
  3
  7
  put 1 10
  put 2 20
  get 1
  put 3 30
  put 4 40
  get 2
  get 4
  After put 3 the cache is {1,2,3}; put 4 evicts 2 (the LRU after get
  1 refreshed 1). get 2 -> MISS (-1); get 4 -> 40 (just inserted, MRU).
  Final size stays 3 of 3 even though 4 keys were touched.

SPECIAL CASES:
  - Capacity 1: every put different from the current key evicts it.
  - Updating an existing key must NOT evict anything and must refresh
    recency - the classic bug is inserting a duplicate node.
  - get on a missing key returns -1 and changes nothing (a miss does
    not refresh anything).
  - Eviction must remove the key from the HASH TABLE as well as the
    list, otherwise a stale index is found later. Erase uses a
    tombstone so other keys in the same probe chain stay findable.
  - Capacity 1..200, operations 1..1000, keys and values are integers.

COMPLEXITY:
  EXPECTED O(1) per operation with a low-load open-addressing table and
  O(capacity) space (never proportional to the number of operations).
  Worst case is O(capacity) per probe if every key collides - say so
  explicitly; hash tables do not promise O(1) worst case.
  The optional on-screen TRACE prints the whole cache and is therefore
  O(capacity) per shown operation; it is disabled automatically for
  capacity > TRACE_LIMIT so the demonstrated algorithm keeps its
  O(1) claim. A display cost is not an algorithmic one - do not confuse
  the two when stating complexity to an interviewer.

TIPS:
  "O(1) get and O(1) recency update" is the trigger phrase for this
  exact pairing: hash map for lookup, doubly linked list for order.
  Singly linked lists cannot reorder in O(1) because they cannot walk
  backwards. Evergreen detail: sentinel head/tail nodes remove EVERY
  null check from the pointer surgery. If an interviewer asks "what
  breaks if you just clear the slot on delete?", the answer is the
  probe chain - that is what the self-test below proves.
*/

#include <stdio.h>
#include <string.h>

#define MAX_CAPACITY 200
#define MAX_OPERATIONS 1000
#define TABLE_SIZE 1024          /* > 4 * capacity: keeps probes short */
#define TRACE_LIMIT 20           /* print the cache only when small */

/* Slot states for the open-addressed table. */
#define SLOT_EMPTY 0
#define SLOT_USED 1
#define SLOT_DELETED 2

int nodeKey[MAX_CAPACITY + 2];
int nodeValue[MAX_CAPACITY + 2];
int nodePrev[MAX_CAPACITY + 2];
int nodeNext[MAX_CAPACITY + 2];

int tableKey[TABLE_SIZE];
int tableIndex[TABLE_SIZE];
unsigned char tableState[TABLE_SIZE];

/* Recycles node indices freed by eviction or by cacheInit(). */
int freeNodes[MAX_CAPACITY + 2];
int freeCount;

int head;                        /* sentinel: MRU side */
int tail;                        /* sentinel: LRU side */
int capacity;
int size;

unsigned long long hashKey(int key) {
    unsigned long long x = (unsigned long long)(unsigned int)key;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return x;
}

int tableFind(int key) {
    size_t index = (size_t)(hashKey(key) % TABLE_SIZE);
    while (tableState[index] != SLOT_EMPTY) {
        if (tableState[index] == SLOT_USED && tableKey[index] == key) {
            return tableIndex[index];
        }
        index = (index + 1) % TABLE_SIZE;      /* walk through tombstones */
    }
    return -1;
}

void tableErase(int key) {
    size_t index = (size_t)(hashKey(key) % TABLE_SIZE);
    while (tableState[index] != SLOT_EMPTY) {
        if (tableState[index] == SLOT_USED && tableKey[index] == key) {
            tableState[index] = SLOT_DELETED;  /* tombstone, NOT SLOT_EMPTY */
            return;
        }
        index = (index + 1) % TABLE_SIZE;
    }
}

void tableInsert(int key, int node) {
    size_t index = (size_t)(hashKey(key) % TABLE_SIZE);
    size_t firstDeleted = (size_t)-1;
    while (tableState[index] != SLOT_EMPTY) {
        if (tableState[index] == SLOT_USED && tableKey[index] == key) {
            tableIndex[index] = node;          /* update in place */
            return;
        }
        if (tableState[index] == SLOT_DELETED && firstDeleted == (size_t)-1) {
            firstDeleted = index;              /* reusable tombstone */
        }
        index = (index + 1) % TABLE_SIZE;
    }
    if (firstDeleted != (size_t)-1) index = firstDeleted;
    tableState[index] = SLOT_USED;
    tableKey[index] = key;
    tableIndex[index] = node;
}

void listDetach(int node) {
    nodeNext[nodePrev[node]] = nodeNext[node];
    nodePrev[nodeNext[node]] = nodePrev[node];
}

void listPushFront(int node) {
    nodePrev[node] = head;
    nodeNext[node] = nodeNext[head];
    nodePrev[nodeNext[head]] = node;
    nodeNext[head] = node;
}

void moveToFront(int node) {
    listDetach(node);
    listPushFront(node);
}

void cacheInit(int newCapacity) {
    capacity = newCapacity;
    size = 0;
    head = 0;
    tail = 1;
    nodePrev[head] = -1;
    nodeNext[head] = tail;
    nodePrev[tail] = head;
    nodeNext[tail] = -1;
    memset(tableState, SLOT_EMPTY, sizeof(tableState));
    /* Every node index 2..capacity+1 is available exactly once. */
    freeCount = 0;
    for (int node = 2; node <= capacity + 1; node++) {
        nodeNext[node] = -1;
        nodePrev[node] = -1;
        freeNodes[freeCount++] = node;
    }
}

/* Returns 1 and stores the value on a hit (refreshing recency);
 * returns 0 on a miss without changing anything. */
int cacheGet(int key, int* outValue) {
    int node = tableFind(key);
    if (node == -1) return 0;
    moveToFront(node);
    *outValue = nodeValue[node];
    return 1;
}

/* Inserts or updates; returns the evicted key, or -1 if none. */
int cachePut(int key, int value) {
    int node = tableFind(key);
    if (node != -1) {
        nodeValue[node] = value;
        moveToFront(node);
        return -1;
    }
    int evicted = -1;
    if (size == capacity) {
        int victim = nodePrev[tail];
        evicted = nodeKey[victim];
        listDetach(victim);
        tableErase(nodeKey[victim]);
        freeNodes[freeCount++] = victim;       /* recycle the node */
        size--;
    }
    int fresh = freeNodes[--freeCount];
    nodeKey[fresh] = key;
    nodeValue[fresh] = value;
    listPushFront(fresh);
    tableInsert(key, fresh);
    size++;
    return evicted;
}

void printCache(void) {
    printf("   MRU ->");
    for (int node = nodeNext[head]; node != tail; node = nodeNext[node]) {
        printf(" [%d:%d]", nodeKey[node], nodeValue[node]);
    }
    printf(" <- LRU\n");
}

/* ------------------------------------------------------------------ */
/* Regression 1: probe-chain deletion on a deliberately tiny table.   */
/* Keys 0, 7, 14 all map to slot 0 of a 7-slot table, so the chain is */
/* 0 -> 1 -> 2. Erasing the middle key must NOT hide the third.       */
/* ------------------------------------------------------------------ */
static unsigned char smallState[7];
static int smallKey[7];

static int smallFind(int key) {
    size_t index = (size_t)key % 7;
    while (smallState[index] != SLOT_EMPTY) {
        if (smallState[index] == SLOT_USED && smallKey[index] == key) {
            return (int)index;
        }
        index = (index + 1) % 7;
    }
    return -1;
}

static void smallInsert(int key) {
    size_t index = (size_t)key % 7;
    while (smallState[index] == SLOT_USED) index = (index + 1) % 7;
    smallState[index] = SLOT_USED;
    smallKey[index] = key;
}

static void smallErase(int key) {
    int index = smallFind(key);
    if (index >= 0) smallState[index] = SLOT_DELETED;   /* tombstone */
}

/* The naive implementation, kept ONLY to show what breaks. */
static void smallEraseBuggy(int key) {
    int index = smallFind(key);
    if (index >= 0) smallState[index] = SLOT_EMPTY;     /* WRONG */
}

static int probeChainSelfTest(void) {
    memset(smallState, SLOT_EMPTY, sizeof(smallState));
    smallInsert(0);
    smallInsert(7);
    smallInsert(14);
    smallErase(7);
    int correct = (smallFind(0) >= 0) && (smallFind(14) >= 0) &&
                  (smallFind(7) == -1);

    memset(smallState, SLOT_EMPTY, sizeof(smallState));
    smallInsert(0);
    smallInsert(7);
    smallInsert(14);
    smallEraseBuggy(7);
    int buggyHidesKey = (smallFind(14) == -1);   /* demonstrates the bug */

    printf("Probe-chain deletion: tombstone keeps key 14 reachable = %s\n",
           correct ? "PASS" : "FAIL");
    printf("Naive slot clearing (documented bug) hides key 14 = %s\n",
           buggyHidesKey ? "confirmed - hence the tombstone" : "not observed");
    return correct;
}

/* ------------------------------------------------------------------ */
/* Regression 2: the real cache vs a slow but obviously-correct        */
/* reference model over 1000 operations.                               */
/* ------------------------------------------------------------------ */
static int refKey[MAX_CAPACITY];
static int refValue[MAX_CAPACITY];
static int refCount;

static void refFindToMru(int position) {
    int key = refKey[position];
    int value = refValue[position];
    for (int i = position; i < refCount - 1; i++) {
        refKey[i] = refKey[i + 1];
        refValue[i] = refValue[i + 1];
    }
    refKey[refCount - 1] = key;                 /* MRU is the last slot */
    refValue[refCount - 1] = value;
}

static int refGet(int key, int* outValue) {
    for (int i = 0; i < refCount; i++) {
        if (refKey[i] == key) {
            *outValue = refValue[i];
            refFindToMru(i);
            return 1;
        }
    }
    return 0;
}

static void refPut(int key, int value) {
    for (int i = 0; i < refCount; i++) {
        if (refKey[i] == key) {
            refValue[i] = value;
            refFindToMru(i);
            return;
        }
    }
    if (refCount == capacity) {                 /* drop the LRU (index 0) */
        for (int i = 0; i < refCount - 1; i++) {
            refKey[i] = refKey[i + 1];
            refValue[i] = refValue[i + 1];
        }
        refCount--;
    }
    refKey[refCount] = key;
    refValue[refCount] = value;
    refCount++;
}

static int compareCacheWithReference(void) {
    if (refCount != size) return 0;
    int node = nodePrev[tail];                   /* LRU first */
    for (int i = 0; i < refCount; i++) {
        if (node == head) return 0;
        if (nodeKey[node] != refKey[i] || nodeValue[node] != refValue[i]) {
            return 0;
        }
        node = nodePrev[node];
    }
    return node == head;
}

static int cacheVersusReferenceSelfTest(void) {
    static const int capacities[] = {1, 2, 3, 5, 7, 20};
    unsigned int seed = 20240517u;
    int allPassed = 1;
    const int operationCount = MAX_OPERATIONS;
    for (size_t c = 0; c < sizeof(capacities) / sizeof(capacities[0]); c++) {
        cacheInit(capacities[c]);
        refCount = 0;
        for (int i = 0; i < operationCount; i++) {
            seed = seed * 1103515245u + 12345u;
            int key = (int)((seed >> 16) % 8u);   /* few keys -> reuse */
            seed = seed * 1103515245u + 12345u;
            int value = (int)((seed >> 16) % 1000u);
            if (((seed >> 8) & 1u) == 0) {
                int cacheValue = 0;
                int refValueOut = 0;
                int cacheHit = cacheGet(key, &cacheValue);
                int refHit = refGet(key, &refValueOut);
                if (cacheHit != refHit ||
                    (cacheHit && cacheValue != refValueOut)) {
                    printf("MISMATCH in get (capacity %d, step %d, key %d)\n",
                           capacities[c], i, key);
                    allPassed = 0;
                    break;
                }
            } else {
                refPut(key, value);
                cachePut(key, value);
            }
            if (!compareCacheWithReference()) {
                printf("MISMATCH in cache state (capacity %d, step %d)\n",
                       capacities[c], i);
                allPassed = 0;
                break;
            }
        }
        if (!allPassed) break;
    }
    printf("%d operations vs the reference model, capacities "
           "1/2/3/5/7/20 = %s\n",
           operationCount, allPassed ? "PASS" : "FAIL");
    return allPassed;
}

int main(void) {
    printf("LRU Cache Design - List plus Hash Map\n\n");
    printf("Built-in regression tests:\n");
    int testsPassed = probeChainSelfTest();
    testsPassed = cacheVersusReferenceSelfTest() && testsPassed;
    printf("\n");

    int chosenCapacity;
    printf("Cache capacity (1-%d): ", MAX_CAPACITY);
    if (scanf("%d", &chosenCapacity) != 1 || chosenCapacity < 1 ||
        chosenCapacity > MAX_CAPACITY) {
        printf("Invalid capacity\n");
        return 1;
    }
    cacheInit(chosenCapacity);
    int trace = chosenCapacity <= TRACE_LIMIT;
    if (!trace) {
        printf("(capacity %d > %d: the per-operation cache trace is "
               "suppressed,\n because printing the cache costs "
               "O(capacity) per line)\n",
               chosenCapacity, TRACE_LIMIT);
    }

    int operations;
    printf("How many operations (1-%d)? ", MAX_OPERATIONS);
    if (scanf("%d", &operations) != 1 || operations < 1 ||
        operations > MAX_OPERATIONS) {
        printf("Invalid operation count\n");
        return 1;
    }
    printf("Enter operations as 'get key' or 'put key value':\n");
    for (int i = 0; i < operations; i++) {
        char command[8];
        int key;
        if (scanf("%7s %d", command, &key) != 2) {
            printf("Invalid operation\n");
            return 1;
        }
        if (strcmp(command, "get") == 0) {
            int value = 0;
            if (cacheGet(key, &value)) {
                printf("get %d -> %d (hit, now MRU)\n", key, value);
            } else {
                printf("get %d -> -1 (miss)\n", key);
            }
        } else if (strcmp(command, "put") == 0) {
            int value;
            if (scanf("%d", &value) != 1) {
                printf("Invalid value\n");
                return 1;
            }
            int before = size;
            int evicted = cachePut(key, value);
            if (evicted != -1) {
                printf("put %d -> evicts LRU %d, inserted (%d/%d in cache)\n",
                       key, evicted, size, capacity);
            } else if (before == size) {
                printf("put %d -> updated (no eviction)\n", key);
            } else {
                printf("put %d -> inserted (%d/%d in cache)\n", key, size,
                       capacity);
            }
        } else {
            printf("Unknown operation \"%s\" (use get or put)\n", command);
            return 1;
        }
        if (trace) printCache();
    }
    printf("\nFinal cache size = %d of %d (distinct nodes ever used = %d)\n",
           size, capacity, size + freeCount);
    printf("Self-check summary: %s\n",
           testsPassed ? "all tests PASS" : "a test FAILED");
    return testsPassed ? 0 : 1;
}
