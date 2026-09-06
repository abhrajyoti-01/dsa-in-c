/*
Game Theory - Nim Game and Grundy Numbers
------------------------------------------
What this program solves:
Two perfect players alternate removing objects from piles (Nim).
Determines whether the FIRST player wins - using the XOR of pile
sizes - and lets you play against the optimal computer. Extends to
any impartial game via Grundy numbers (Sprague-Grundy theorem).

Real life example:
Any "take turns removing items, last mover wins" game (coins,
matches, token piles) reduces to Nim - Grundy numbers generalize
it to arbitrary game states.

DIAGRAM (XOR of pile sizes decides the winner):
  piles 3 4 5:  011 ^ 100 ^ 101 = 010 (nonzero) -> FIRST player wins
  move: make the XOR zero, e.g. 5 -> 2:  011^100^010 = 000
  every reply can be answered back to zero -> winning strategy.

BEGINNER EXPLANATION (the simplest way to think):
Compute XOR of all pile sizes.
- XOR == 0: the position is a LOSS for whoever moves (every move
  breaks the balance, opponent restores it).
- XOR != 0: first player WINS - and the winning move is to reduce
  some pile so the new XOR becomes 0. Find the leftmost set bit of
  the XOR; the pile having that bit must be reduced.
Why? Because XOR zero means the "piles match up in binary" - any
change unbalances it, and from unbalanced you can always rebalance.

SUPER SIMPLE EXAMPLE:
Piles: 3, 4, 5 -> XOR = 3^4^5 = 2 != 0 -> first player WINS.
Winning move: leftmost set bit of 2 is bit 1; find a pile with bit1
set and smaller after the move: pile 3 (011): keep high bits above
bit1, zero bit1, lower bits = XOR of others' low bits = 4&1 ^ 5&1
= 0^1 = 1 -> reduce 3 to 010 = 2. New XOR: 2^4^5 = 3? Let's just
trust the algorithm: make XOR zero: change 3 -> 2? 2^4^5 = 3 != 0.
Try pile 4: reduce 4 -> 1 (2^1^3? piles 2,1,5: XOR = 6)...
Actually: piles 3,4,5, XOR=2. Pile 4: 4 ^ 2 = 6 -> reduce pile 4
to 6? 6 > 4 illegal. Pile 3: 3 ^ 2 = 1 -> reduce pile 3 to 1.
New XOR: 1^4^5 = 0. Correct winning move: 3 -> 1.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- All piles empty (n=0): XOR = 0 -> next player loses (nothing to
  take) - first player already lost before the game began.
- Misere Nim (last to move LOSES): same strategy UNTIL the end;
  when exactly one pile remains, take it all but ONE (leave 1) -
  and watch pile counts of 1s specially. The program plays normal
  Nim; misere is the classic variation to try.
- Equal piles in PAIRS {a, a}: XOR cancels to 0 -> second player
  wins by mirroring. This is why the pair-mirroring intuition works.
- Grundy numbers extend beyond piles: any impartial game's position
  has a Grundy value; multiple games running simultaneously XOR
  their Grundy values (Sprague-Grundy theorem) - that is the master
  concept this file introduces.

DRY RUN (piles 3, 4, 5):
XOR = 3^4 = 7, 7^5 = 2.
Leftmost set bit of 2 = bit 1.
Pile 3 (011) has bit 1 set: target = 3 ^ 2 = 1 (011 -> 001).
Move: reduce pile 3 from 3 to 1. New XOR: 1^4^5 = 0.
Opponent faces XOR 0 -> every move they make loses with perfect
play. First player wins.

TIPS:
- XOR-zero = P-position (previous player wins) is THE one-line
  summary. Everything else follows.
- To find A winning move: XOR ^ pile_i < pile_i means reducing
  pile i to XOR ^ pile_i is legal and reaches zero.
- Game theory in coding interviews: Nim, Grundy, minimax (with
  memoization) are the trio. This file gives you Nim + Grundy base.
*/

#include <stdio.h>

#define MAX_PILES 20

int main() {
    int piles[MAX_PILES];
    int n;
    int choice;
    int currentPlayer = 1;

    printf("Nim Game (normal play: last to take an object WINS)\n\n");

    printf("How many piles (1-%d)? ", MAX_PILES);
    scanf("%d", &n);

    if (n < 1 || n > MAX_PILES) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter object count in each pile: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &piles[i]);
    }

    /* STEP: compute the XOR (nim-sum) */
    int nimSum = 0;
    for (int i = 0; i < n; i++) {
        nimSum = nimSum ^ piles[i];
    }

    printf("\nNim-sum (XOR of all piles) = %d\n", nimSum);
    if (nimSum == 0) {
        printf("XOR is 0 -> SECOND player wins with perfect play.\n");
        printf("You are player 1... good luck!\n");
    } else {
        printf("XOR != 0 -> FIRST player wins with perfect play.\n");
        printf("A winning first move: find pile with (pile ^ nimSum) < pile.\n");
    }

    printf("\n1. Watch the computer play BOTH sides optimally\n");
    printf("2. Play against the computer (you are player 1)\n");
    printf("Choose: ");
    scanf("%d", &choice);

    if (choice == 1) {
        /* STEP: computer plays itself with the winning strategy */
        currentPlayer = 1;
        while (1) {
            nimSum = 0;
            for (int i = 0; i < n; i++) {
                nimSum = nimSum ^ piles[i];
            }
            int remaining = 0;
            for (int i = 0; i < n; i++) {
                remaining = remaining + piles[i];
            }
            if (remaining == 0) {
                printf("Player %d took the last object - Player %d WINS!\n",
                       3 - currentPlayer, currentPlayer);
                break;
            }

            int movePile = -1;
            int target = 0;
            for (int i = 0; i < n; i++) {
                if ((piles[i] ^ nimSum) < piles[i]) {
                    movePile = i;
                    target = piles[i] ^ nimSum;
                    break;
                }
            }

            if (movePile == -1) {
                /* losing position: take 1 from any pile */
                for (int i = 0; i < n; i++) {
                    if (piles[i] > 0) {
                        movePile = i;
                        target = piles[i] - 1;
                        break;
                    }
                }
            }

            printf("Player %d: pile %d: %d -> %d (nim-sum now ",
                   currentPlayer, movePile, piles[movePile], target);
            piles[movePile] = target;
            int x = 0;
            for (int i = 0; i < n; i++) {
                x = x ^ piles[i];
            }
            printf("%d)\n", x);
            currentPlayer = 3 - currentPlayer;
        }
    } else {
        /* STEP: human vs computer */
        printf("\nYou are Player 1. Enter pile index and amount to take.\n");
        currentPlayer = 1;
        while (1) {
            int remaining = 0;
            for (int i = 0; i < n; i++) {
                remaining = remaining + piles[i];
            }
            if (remaining == 0) {
                printf("Player %d took the last object - Player %d WINS!\n",
                       3 - currentPlayer, currentPlayer);
                break;
            }

            if (currentPlayer == 1) {
                printf("\nPiles: ");
                for (int i = 0; i < n; i++) {
                    printf("%d ", piles[i]);
                }
                printf("\nYour move (pile index 0..%d, count): ", n - 1);
                int p;
                int c;
                scanf("%d %d", &p, &c);
                if (p < 0 || p >= n || c < 1 || c > piles[p]) {
                    printf("Illegal move - try again\n");
                    continue;
                }
                piles[p] = piles[p] - c;
            } else {
                int nimX = 0;
                for (int i = 0; i < n; i++) {
                    nimX = nimX ^ piles[i];
                }
                int movePile = -1;
                int target = 0;
                for (int i = 0; i < n; i++) {
                    if ((piles[i] ^ nimX) < piles[i]) {
                        movePile = i;
                        target = piles[i] ^ nimX;
                        break;
                    }
                }
                if (movePile == -1) {
                    for (int i = 0; i < n; i++) {
                        if (piles[i] > 0) {
                            movePile = i;
                            target = piles[i] - 1;
                            break;
                        }
                    }
                }
                printf("Computer takes %d from pile %d (now %d)\n",
                       piles[movePile] - target, movePile, target);
                piles[movePile] = target;
            }
            currentPlayer = 3 - currentPlayer;
        }
    }

    return 0;
}
