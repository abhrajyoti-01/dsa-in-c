/*
Reservoir Sampling - Random Selection in a Stream
-------------------------------------------------
What this program solves:
Picks k items UNIFORMLY AT RANDOM from a stream of unknown (possibly
huge) length using only O(k) memory - you cannot store the stream,
yet every item must have probability k/n of being chosen.

Real life example:
Sampling logs or network packets without storing them: pick k
uniformly from an endless stream with only k memory slots - used
by telemetry and A/B systems.

DIAGRAM (stream of unknown length, reservoir of k):
  first k items -> straight into the reservoir
  item i (i > k): keep it with probability k/i, else skip
  proof sketch: P(item survives) = k/i * i/(i+1) * ... = k/n.

BEGINNER EXPLANATION (the simplest way to think):
Keep a "reservoir" (basket) with the first k items. For item i
(i > k), roll a die: pick a random number r in [0, i). If r < k,
swap item i INTO the basket at position r (kicking the old one
out); otherwise ignore it. The math guarantees that when you have
seen n items, every item has exactly k/n chance of being in the
basket - proof by induction: item i survives later swaps with
probability k/(i+1) * (k+1)/(k+2) * ... * n/(n+1) = k/n.

SUPER SIMPLE EXAMPLE (k = 2):
Stream: A, B, C, D
A -> basket [A, B]  (first k fill it)
C -> r random in {0,1}: say r=0 -> basket [C, B]  (C got in with
     probability 2/3, displacing A)
D -> r random in {0,1}: say r=1 -> basket [C, D]
Now D had 2/4 chance to be present - correct proportionality.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Stream shorter than k: every item goes in - trivially uniform.
- k = 1: classic "pick one random line from a file" - probability
  1/n for each line. The single-item case is the whole intuition.
- Same stream, different runs: DIFFERENT samples each run - that
  is expected randomness, not a bug. (Deterministic seed makes it
  reproducible for testing.)
- Weights: uniform sampling ignores item weight - weighted
  versions exist (A-Res algorithm) if items have priorities.

DRY RUN (k = 2, stream A B C D E, lucky rolls r=0 then r=0):
i=0: A in. basket [A]
i=1: B in. basket [A, B]
i=2 (C): r = 0 < 2 -> basket[0] = C. [C, B]
i=3 (D): r = 1 < 2 -> basket[1] = D. [C, D]
i=4 (E): r = 3 >= 2 -> reject. [C, D]
Each item i had probability 2/(i+1) when it arrived, and each
survivor's continued presence is (i+1)/(i+2) per later step, so
final probability for every item = 2/5. Uniform!

TIPS:
- THE use case: sampling from data too big to store (log files,
  sensor streams, huge DBs where ORDER BY RANDOM() is impossible).
- Probability proof in one line: P(item i in final) = (k/i) *
  prod_{j=i+1}^{n} (1 - k/j) = k/n - telescoping cancellation.
- Variants: weighted reservoir (A-Chao), distributed reservoir
  per shard then merge.
- srand(time(NULL)) once at program start - calling it inside a
  loop re-seeds and repeats values (classic bug).
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STREAM 100000

int main() {
    int k;
    int n;
    char again;

    printf("Reservoir Sampling (uniform sample from a stream)\n\n");

    do {
        printf("Sample size k (1-50)? ");
        scanf("%d", &k);

        printf("Simulate stream length n (1-%d)? ", MAX_STREAM);
        scanf("%d", &n);

        if (k < 1 || k > 50 || n < 1 || n > MAX_STREAM) {
            printf("Invalid sizes\n");
            continue;
        }

        /* STEP: reservoir algorithm over a simulated stream */
        int reservoir[50];
        for (int i = 0; i < k; i++) {
            reservoir[i] = i;
        }

        int swaps = 0;
        for (int i = k; i < n; i++) {
            int r = rand() % (i + 1);
            if (r < k) {
                reservoir[r] = i;
                swaps = swaps + 1;
            }
        }

        printf("\nStream items 0..%d\n", n - 1);
        printf("Sample of %d (uniform, each item probability %.4f):\n  ",
               k, (double)k / n);
        for (int i = 0; i < k; i++) {
            printf("%d ", reservoir[i]);
        }
        printf("\nTotal swap-in events during the stream: %d\n", swaps);

        /* STEP: frequency check over many runs (proves uniformity) */
        if (n <= 20 && n > k) {
            int freq[100] = {0};
            int trials = 10000;
            for (int t = 0; t < trials; t++) {
                int res2[50];
                for (int i = 0; i < k; i++) {
                    res2[i] = i;
                }
                for (int i = k; i < n; i++) {
                    int r = rand() % (i + 1);
                    if (r < k) {
                        res2[r] = i;
                    }
                }
                for (int i = 0; i < k; i++) {
                    freq[res2[i]] = freq[res2[i]] + 1;
                }
            }
            printf("Frequency over %d runs (expect ~%.0f each):\n",
                   trials, trials * (double)k / n);
            for (int i = 0; i < n; i++) {
                printf("  item %d: %d times\n", i, freq[i]);
            }
        }

        printf("\nRun again with fresh randomness? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
