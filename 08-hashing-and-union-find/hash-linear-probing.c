/*
Hash Table - Open Addressing (Linear Probing)
---------------------------------------------
What this program solves:
The OTHER collision strategy (vs separate chaining in
hash_table.c): all keys live INSIDE one array; a collision walks
forward slot by slot until a free one appears. Includes delete
with tombstones and search termination logic.

Real life example:
CPU caches and embedded hash tables can afford extra list nodes -
they store entries IN the array and probe forward, exactly like
linear probing (and tombstones mark deleted slots).

DIAGRAM (insert 25, 35 into table size 10, key % 10):
  25 -> slot 5 (empty)          [.. .. .. .. .. 25 .. .. .. ..]
  35 -> slot 5 taken -> slot 6  [.. .. .. .. .. 25 35 .. .. ..]
  search 35: slot 5 has 25 -> KEEP WALKING -> found at 6
  delete 25 -> tombstone, or 35 would become unreachable.

BEGINNER EXPLANATION (the simplest way to think):
A parking lot with numbered spots. Your key decides your preferred
spot: hash(key). If it is taken, drive to the NEXT spot, then the
next... until you find a free one. To find your car later, replay
the same walk. DELETING is the twist: you cannot just empty the
spot - cars that parked further along (because yours was there)
would become unfindable. So leave a "reserved" marker: a
TOMBSTONE.

SUPER SIMPLE EXAMPLE (table size 7, hash = key % 7):
insert 8 -> spot 1. insert 15 -> spot 1 taken -> spot 2.
insert 1 -> spot 1 taken -> 2 taken -> spot 3.
Now the table: [_, 8, 15, 1, _, _, _]
search 1: hash 1 -> 8 no -> 15 no -> 1 YES (3 probes)
delete 8 -> place TOMBSTONE at spot 1.
search 1: spot 1 tombstone -> KEEP GOING -> 15 -> 1 YES. Works!

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Table FULL: insertion impossible -> "resize" (rehash into a
  bigger table). This program reports overflow instead.
- Tombstone blocks: too many deletes without inserts fills the
  probe chains with tombstones -> search slows. Real systems
  rehash when tombstones exceed half the table.
- Clustering: linear probing builds long "runs" (primary
  clustering) - consecutive occupied spots grow together. That is
  why average probes degrade to O(n) when the load factor passes
  ~0.7, even though the table is not full.
- Load factor > 1 is impossible here (unlike chaining).

DRY RUN (insert 10, 17, 24 into size 7, all hash to 3):
10 -> spot 3 free -> place. [_,_,_,10,_,_,_]
17 -> spot 3 taken -> 4 free -> place. [_,_,_,10,17,_,_]
24 -> spot 3,4 taken -> 5 free -> place. [_,_,_,10,17,24,_]
search 24: 3(taken,10) -> 4(17) -> 5(24) found in 3 probes.
delete 17 -> tombstone at 4.
search 24: 3(10) -> 4(TOMBSTONE, continue!) -> 5(24) found.

TIPS:
- Chaining vs probing: chaining never overflows and deletes are
  simple; probing is cache-friendlier and uses no pointers. Know
  both - interviews ask to compare them.
- Probe sequence variants: quadratic probing (1,4,9,...) and
  double hashing reduce clustering. Try modifying the step.
- The invariant to never break: search must terminate - either a
  match, an EMPTY (never-used) slot, or a full-table loop. Only
  EMPTY stops search; TOMBSTONE never does.
*/

#include <stdio.h>

#define TABLE_SIZE 11
#define EMPTY 0
#define OCCUPIED 1
#define TOMBSTONE 2

typedef struct {
    int key;
    int value;
    int state;
} Slot;

Slot table[TABLE_SIZE];
int itemCount = 0;

/*
 * hashFunction - Division method.
 */
int hashFunction(int key) {
    int h = key % TABLE_SIZE;
    if (h < 0) {
        h = h + TABLE_SIZE;
    }
    return h;
}

/*
 * insertKey - Linear probe for a free or tombstone slot.
 */
void insertKey(int key, int value) {
    if (itemCount >= TABLE_SIZE - 1) {
        printf("Table nearly full - insert refused (rehash in real systems)\n");
        return;
    }

    int idx = hashFunction(key);
    int firstTombstone = -1;

    for (int probes = 0; probes < TABLE_SIZE; probes++) {
        int slot = (idx + probes) % TABLE_SIZE;

        if (table[slot].state == OCCUPIED) {
            if (table[slot].key == key) {
                table[slot].value = value;
                printf("Key %d UPDATED to %d (slot %d)\n", key, value, slot);
                return;
            }
        } else if (table[slot].state == TOMBSTONE) {
            if (firstTombstone == -1) {
                firstTombstone = slot;
            }
        } else {
            /* EMPTY: key is definitely not in the table */
            int target = (firstTombstone != -1) ? firstTombstone : slot;
            table[target].key = key;
            table[target].value = value;
            table[target].state = OCCUPIED;
            itemCount = itemCount + 1;
            printf("Inserted (%d -> %d) at slot %d after %d probe(s)\n",
                   key, value, target, probes);
            return;
        }
    }
    printf("Table full - cannot insert %d\n", key);
}

/*
 * searchKey - Stops only at EMPTY (tombstones never stop search).
 */
void searchKey(int key) {
    int idx = hashFunction(key);

    for (int probes = 0; probes < TABLE_SIZE; probes++) {
        int slot = (idx + probes) % TABLE_SIZE;

        if (table[slot].state == EMPTY) {
            printf("Key %d NOT FOUND (%d probe(s), hit empty slot)\n",
                   key, probes + 1);
            return;
        }
        if (table[slot].state == OCCUPIED && table[slot].key == key) {
            printf("Key %d -> value %d at slot %d (%d probe(s))\n",
                   key, table[slot].value, slot, probes + 1);
            return;
        }
        /* TOMBSTONE or wrong key: keep probing */
    }
    printf("Key %d NOT FOUND (probed whole table)\n", key);
}

/*
 * deleteKey - Leaves a tombstone so later probes still pass.
 */
void deleteKey(int key) {
    int idx = hashFunction(key);

    for (int probes = 0; probes < TABLE_SIZE; probes++) {
        int slot = (idx + probes) % TABLE_SIZE;

        if (table[slot].state == EMPTY) {
            break;
        }
        if (table[slot].state == OCCUPIED && table[slot].key == key) {
            table[slot].state = TOMBSTONE;
            itemCount = itemCount - 1;
            printf("Deleted key %d (slot %d now a tombstone)\n", key, slot);
            return;
        }
    }
    printf("Key %d not found\n", key);
}

/*
 * displayTable - States, keys, and the current clustering.
 */
void displayTable(void) {
    printf("\nSlot | State     | Key   | Value\n");
    printf("-----+-----------+-------+-------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        const char* stateName =
            (table[i].state == EMPTY) ? "EMPTY" :
            (table[i].state == TOMBSTONE) ? "TOMBSTONE" : "OCCUPIED";
        if (table[i].state == OCCUPIED) {
            printf("%4d | %-9s | %5d | %5d\n", i, stateName,
                   table[i].key, table[i].value);
        } else {
            printf("%4d | %-9s |       |       \n", i, stateName);
        }
    }
    printf("Items: %d / %d, load factor: %.2f\n",
           itemCount, TABLE_SIZE, (double)itemCount / TABLE_SIZE);
}

int main() {
    int choice;
    int key;
    int value;

    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].state = EMPTY;
    }

    printf("Hash Table - Open Addressing (Linear Probing)\n");
    printf("Table size %d, hash = key %% %d\n\n", TABLE_SIZE, TABLE_SIZE);

    while (1) {
        printf("\n1. Insert key-value\n");
        printf("2. Search key\n");
        printf("3. Delete key (tombstone)\n");
        printf("4. Display table\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter key and value: ");
            scanf("%d %d", &key, &value);
            insertKey(key, value);
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

    return 0;
}
