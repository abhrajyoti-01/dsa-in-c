<div align="center">

# 🚀 Data Structures & Algorithms in C

### A Complete Learning Journey — From Zero to Mastery

**198 programs · 20 modules · 1 LeetCode/HackerRank collection**

[![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Programs](https://img.shields.io/badge/Programs-198-2ea44f?style=for-the-badge)]()
[![Modules](https://img.shields.io/badge/Modules-20-8A2BE2?style=for-the-badge)]()
[![Platform Problems](https://img.shields.io/badge/LeetCode%20%2F%20HackerRank-31-1f6feb?style=for-the-badge)]()
[![Compiler](https://img.shields.io/badge/Compiler-gcc%20--Wall%20--Wextra-FF6C37?style=for-the-badge&logo=gnu&logoColor=white)]()
[![License](https://img.shields.io/badge/License-MIT-4B8BBE?style=for-the-badge)]()

*Every program is input-driven, self-documented, and built to be understood.*

</div>

---

## 📖 Table of Contents

- [✨ Why This Repository](#-why-this-repository)
- [🎯 Who Is This For](#-who-is-this-for)
- [🧭 The Roadmap](#-the-roadmap)
- [📂 Repository Structure](#-repository-structure)
- [🎯 LeetCode & HackerRank Collection](#-leetcode--hackerrank-collection)
- [🚦 Learning Stages](#-learning-stages)
- [🛠️ Getting Started](#️-getting-started)
- [📚 How Each Program Teaches](#-how-each-program-teaches)
- [⚡ Complexity Cheat Sheet](#-complexity-cheat-sheet)
- [🧠 Beyond the Code](#-beyond-the-code)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## ✨ Why This Repository

Most algorithm repositories dump code. This one **teaches**.

Every one of the **198 programs** is a complete, self-contained lesson that includes:

| 🧩 Component | What it gives you |
|---|---|
| **Problem Definition** | What we are solving and why it matters |
| **Real-Life Example** | The everyday situation the algorithm models |
| **ASCII Diagram** | A visual of the data structure or process |
| **Pseudo Code** | The logic in plain language, before C syntax |
| **Dry Run** | A step-by-step trace of a small example |
| **Special Cases** | Edge cases where the result may surprise you |
| **Tips** | Interview points, classic bugs, and what to learn next |

> 💡 **The philosophy:** if you can explain it, you can code it. Every file is written so you can explain it.

---

## 🎯 Who Is This For

- 🎓 **Students** preparing for DSA exams, lab records, and viva
- 💼 **Job seekers** building interview fundamentals
- 🧑‍💻 **Self-learners** who want structure, not scattered snippets
- 👨‍🏫 **Teachers** looking for a ready-made, ordered curriculum

**Prerequisites:** basic C syntax (variables, loops, functions). Everything else is built from here, step by step.

---

## 🧭 The Roadmap

```
FOUNDATIONS ──► CORE TECHNIQUES ──► HIERARCHICAL DATA ──► DESIGN PARADIGMS ──► ADVANCED ──► MASTER
     │                 │                   │                   │                 │            │
 01-fundamentals   03-searching        07-trees-and-heaps   09-greedy        12-graphs    16-network-flow
 02-recursion      04-sorting          08-hashing-union     10-divide-       13-backtracking 17-geometry
                  05-stacks-queues        find               conquer         14-branch-bound 18-randomized
                  06-linked-lists      11-dynamic-          15-string-       19-bitmask-dp
                                        programming          algorithms      20-advanced-c
```

**The rule:** each module assumes only the modules before it. Follow the numbers — they are the order of learning.

**Beside the curriculum:** [`leetcode_hackerrank_problems/`](#-leetcode--hackerrank-collection) holds the 31 practice lessons whose headers are keyed to numbered LeetCode and HackerRank problems.

---

## 📂 Repository Structure

### 🟢 Level 1 — Foundations

| Module | Focus | Programs |
|:---:|:---|:---|
| **01-fundamentals** | C building blocks | array-basics, array-problems, subarray-techniques, difference-array, number-checks, pattern-printing, swap-and-conversions, structure-basics, pointer-basics, pointer-operations, dynamic-memory, string-operations, bit-manipulation, matrix-operations, spiral-matrix |
| **02-recursion-and-mathematics** | Think recursively | factorial, recursion-techniques, tower-of-hanoi, string-permutations, pascal-triangle, josephus, gcd-euclid, modular-inverse, fast-power, sieve-of-eratosthenes, math-techniques |

### 🟡 Level 2 — Core Techniques

| Module | Focus | Programs |
|:---:|:---|:---|
| **03-searching** | From O(n) to O(log n) | linear-search, binary-search, binary-search-variants, advanced-search-problems, jump-search, ternary-search, interpolation-search |
| **04-sorting** | Every major sort | bubble, selection, insertion, shell, merge, quick, heap, counting, bucket, radix, sorting-problems, structure-sort-quickselect |
| **05-stacks-queues-deques** | Linear ADTs & their power | stack, queue, deque, balanced-parentheses, infix-to-postfix, postfix-evaluation, stack-variants, queue-variants, next-greater-stock-span, largest-rectangle-histogram, sliding-window-maximum, deque-applications |
| **06-linked-lists** | Pointer surgery | singly, doubly, circular, linked-list-problems, polynomial-addition |

### 🟠 Level 3 — Hierarchical Data

| Module | Focus | Programs |
|:---:|:---|:---|
| **07-trees-and-heaps** | Trees, from plain to advanced | heap, binary-tree, bst, bst-problems, avl-tree, red-black-tree, b-tree, threaded-bst, trie, expression-tree, splay-tree, segment-tree, lazy-segment-tree, sparse-table, fenwick-tree, heap-applications, tree-problems-advanced |
| **08-hashing-and-union-find** | O(1) tricks & set magic | hash-table, hash-linear-probing, hash-problems, union-find, union-find-applications |

### 🔵 Level 4 — Design Paradigms

| Module | Focus | Programs |
|:---:|:---|:---|
| **09-greedy** | Local optimum → global optimum | activity-selection, coin-change-greedy, knapsack-fractional, job-sequencing, minimum-platforms, intervals-greedy, huffman-coding |
| **10-divide-and-conquer** | Split, solve, merge | min-max, closest-pair, inversions-median, strassen |
| **11-dynamic-programming** | Overlapping subproblems | fibonacci, climbing-stairs-house-robber, grid-dp, kadane, coin-change-dp, knapsack-0-1, lcs, lis, edit-distance, matrix-chain-multiplication, optimal-bst, rod-cutting, partition-equal-sum, palindromic-subsequence, egg-dropping, boolean-parenthesization, subset-sum-problems, weighted-interval-scheduling |

### 🔴 Level 5 — Advanced

| Module | Focus | Programs |
|:---:|:---|:---|
| **12-graphs** | The full graph toolkit | graph-problems, bfs, dfs, topo-sort, connected-components, dijkstra, zero-one-bfs, bellman-ford, floyd-warshall, warshall, prim, kruskal, kosaraju-scc, bridges-articulation, bipartite-check, euler-path, grid-graph-problems, lca-binary-lifting |
| **13-backtracking** | Explore, choose, undo | subsets-combinations-word-search, n-queens, sum-of-subsets, rat-in-maze, knights-tour, sudoku-solver, hamiltonian-cycle, graph-coloring, tsp-backtracking |
| **14-branch-and-bound** | Search with pruning | knapsack-branch-bound, tsp-branch-and-bound, fifteen-puzzle-solver |
| **15-string-algorithms** | Pattern matching done right | naive-string-matching, kmp-string-matching, z-algorithm, rabin-karp, boyer-moore, manacher, string-problems, string-traversal-techniques, string-sorting-patterns |

### ⚫ Level 6 — Master

| Module | Focus | Programs |
|:---:|:---|:---|
| **16-network-flow** | Flow & matching | max-flow, dinic-max-flow, bipartite-matching |
| **17-computational-geometry** | Geometry on a plane | convex-hull, line-segment-intersection, point-in-polygon |
| **18-randomized-algorithms** | Probability as a tool | miller-rabin, reservoir-sampling |
| **19-bitmask-dp-game-theory** | Subset DP & games | tsp-bitmask-dp, assignment-bitmask-dp, catalan-numbers, nim-game |
| **20-advanced-c-programming** | Master-level C | function-pointers, generic-containers, file-handling |

### New lessons — basic to advanced

Each addition follows the same lesson format, with runnable sample input,
an exact dry run, complexity analysis, and explicit input limits.

| Level | Program | What you learn |
|:---|:---|:---|
| Basic → Intermediate | difference-array | Batch range additions in O(n + q) |
| Intermediate | modular-inverse | Modular division using extended Euclid, including composite moduli |
| Intermediate | largest-rectangle-histogram | Monotonic stacks and nearest-smaller boundaries |
| Advanced | sparse-table | O(1) static range minimum after O(n log n) preprocessing |
| Advanced | lazy-segment-tree | Interleaved range additions and sums in O(log n) |
| Advanced | weighted-interval-scheduling | Profit-based scheduling, binary search, and solution reconstruction |
| Advanced | zero-one-bfs | O(V + E) shortest paths for weights 0 and 1 |
| Advanced | lca-binary-lifting | Common ancestors and distances in general rooted trees |
| Advanced | manacher | O(n) longest palindromic substring, including even lengths |
| Master | dinic-max-flow | Level graphs, blocking flows, and a minimum-cut certificate |
| Intermediate → Advanced | point-in-polygon | Exact inside/outside/boundary tests for concave polygons |
| Master | assignment-bitmask-dp | O(n·2ⁿ) minimum-cost assignment with reconstruction |

**Suggested progression:** difference-array → sparse-table → lazy-segment-tree
for range techniques; bfs → zero-one-bfs → lca-binary-lifting for graph queries;
max-flow → dinic-max-flow for flow networks.

---

## 🎯 LeetCode & HackerRank Collection

**Folder: [`leetcode_hackerrank_problems/`](leetcode_hackerrank_problems) · 31 lessons**

Every lesson below is a normal, self-contained, input-driven C program — it just
happens to answer one or more **numbered platform problems**. Each file's header
starts with a `PLATFORM PROBLEMS:` block naming the exact LeetCode numbers and
HackerRank titles it solves, so you can practise the platform question and then
read the C implementation side by side.

```bash
cd leetcode_hackerrank_problems
gcc -Wall -Wextra -o lru lru-cache-design.c && ./lru
```

| Lesson | LeetCode | HackerRank |
|:---|:---|:---|
| [prefix-sum-2d](leetcode_hackerrank_problems/prefix-sum-2d.c) | 304, 1314 | 2D Array – DS |
| [two-pointers](leetcode_hackerrank_problems/two-pointers.c) | 167, 125, 344, 977, 88 | Print in Reverse |
| [digit-math](leetcode_hackerrank_problems/digit-math.c) | 9, 258, 7 | Recursive Digit Sum |
| [combinatorics](leetcode_hackerrank_problems/combinatorics.c) | 62, 96, 118 | nCr / Handshake |
| [binary-search-on-answer](leetcode_hackerrank_problems/binary-search-on-answer.c) | 875, 1011, 410, 69 | Cutting Paper Squares |
| [matrix-search](leetcode_hackerrank_problems/matrix-search.c) | 74, 240, 378 | Matrix |
| [median-two-sorted](leetcode_hackerrank_problems/median-two-sorted.c) | 4, 21 | Find the Median |
| [interval-problems](leetcode_hackerrank_problems/interval-problems.c) | 56, 57, 252, 253, 1288 | Interval scheduling |
| [hackerrank-sorting-classics](leetcode_hackerrank_problems/hackerrank-sorting-classics.c) | 435, 1029, 1710 | Big Sorting · Mark and Toys · Maximum Perimeter Triangle · Priyanka and Toys |
| [monotonic-stack](leetcode_hackerrank_problems/monotonic-stack.c) | 496, 739, 84, 42, 901 | Largest Rectangle |
| [sliding-window](leetcode_hackerrank_problems/sliding-window.c) | 3, 76, 209, 424, 567, 643 | Substring Diff |
| [lru-cache-design](leetcode_hackerrank_problems/lru-cache-design.c) | 146, 460, 706, 707 | Cycle Detection (design drills) |
| [tree-interview-problems](leetcode_hackerrank_problems/tree-interview-problems.c) | 104, 110, 543, 226, 112, 236, 102 | Tree: Height of a Binary Tree |
| [bst-interview-problems](leetcode_hackerrank_problems/bst-interview-problems.c) | 98, 230, 700, 701, 938, 450 | BST: Insertion / Lowest Common Ancestor |
| [hashmap-interview-problems](leetcode_hackerrank_problems/hashmap-interview-problems.c) | 1, 128, 49, 560, 217 | Pairs / Count Triplets |
| [union-find-interview-problems](leetcode_hackerrank_problems/union-find-interview-problems.c) | 547, 684, 721, 959 | Merging Communities · Kundu and Tree |
| [stock-dp](leetcode_hackerrank_problems/stock-dp.c) | 121, 122, 123, 188, 309, 714 | Stock Maximize |
| [dp-interview-problems](leetcode_hackerrank_problems/dp-interview-problems.c) | 91, 139, 322, 279, 300, 44 | Maximum Subarray / Coin Change |
| [grid-bfs-paths](leetcode_hackerrank_problems/grid-bfs-paths.c) | 1091, 1926, 994, 542, 200 | Shortest Reach in a Graph |
| [course-schedule-topo](leetcode_hackerrank_problems/course-schedule-topo.c) | 207, 210, 269, 802 | Topological Sort |
| [word-ladder-bfs](leetcode_hackerrank_problems/word-ladder-bfs.c) | 127, 126, 433 | BFS: Shortest Reach |
| [backtracking-interview-problems](leetcode_hackerrank_problems/backtracking-interview-problems.c) | 46, 39, 17, 22, 78 | N-Queens family |
| [string-interview-problems](leetcode_hackerrank_problems/string-interview-problems.c) | 8, 20, 443, 14, 151, 125 | Super Reduced String |
| [bit-tricks-interview](leetcode_hackerrank_problems/bit-tricks-interview.c) | 136, 137, 191, 338, 231, 78 | Lonely Integer · Maximizing XOR |
| [array-hard-interview-problems](leetcode_hackerrank_problems/array-hard-interview-problems.c) | 41, 229, 238, 448 | Minimum Swaps 2 (in-place index idea) |
| [hard-greedy-and-sweep](leetcode_hackerrank_problems/hard-greedy-and-sweep.c) | 218, 621, 402, 316 | Greedy scheduling drills |
| [expression-evaluation-interview](leetcode_hackerrank_problems/expression-evaluation-interview.c) | 227, 224, 394, 71 | Balanced Brackets |
| [divide-conquer-hard](leetcode_hackerrank_problems/divide-conquer-hard.c) | 493, 315, 912 | Merge Sort: Counting Inversions |
| [dp-hard-interval-and-grid](leetcode_hackerrank_problems/dp-hard-interval-and-grid.c) | 312, 221, 85, 152 | The Maximum Subarray |
| [tree-dp-hard](leetcode_hackerrank_problems/tree-dp-hard.c) | 337, 124, 968 | Tree: Height of a Binary Tree |
| [hackerrank-tricky-classics](leetcode_hackerrank_problems/hackerrank-tricky-classics.c) | — | New Year Chaos · Minimum Swaps 2 · Maximum Subarray Sum · Simple Text Editor |

**Suggested progression:** two-pointers → sliding-window → monotonic-stack
for array/string screens; hashmap → union-find → dp-interview-problems for the
hard round; grid-bfs-paths → course-schedule-topo → word-ladder-bfs for graphs.

---

## 🚦 Learning Stages

A curated 32-step path through the repository — some steps group related programs.

### Stage A — Beginner *(pure logic, no fancy structures)*
| # | Program | Core idea |
|:---:|:---|:---|
| 1 | array-basics | shifting mechanics |
| 2 | number-checks | digit loops, sqrt bound |
| 3 | factorial | recursion: base case + progress |
| 4 | linear-search | the O(n) baseline |
| 5 | bubble / selection / insertion sort | comparison sorting |
| 6 | tower-of-hanoi | recursive structure |
| 7 | binary-search | halving on sorted data |

### Stage B — Intermediate *(ADTs & their applications)*
| # | Program | Core idea |
|:---:|:---|:---|
| 8 | balanced-parentheses | order checking with a stack |
| 9 | infix-to-postfix + postfix-evaluation | a complete expression engine |
| 10 | singly-linked-list | pointer surgery |
| 11 | merge / quick sort | recursion + partition |
| 12 | binary-search-variants | boundary thinking |
| 13 | stack / queue / deque | ADT mechanics |
| 14 | bst | ordered tree logic |
| 15 | hash-table + hash-linear-probing | chaining vs probing |
| 16 | greedy: activity / coins / knapsack | proof by exchange |

### Stage C — Advanced *(DP & graphs)*
| # | Program | Core idea |
|:---:|:---|:---|
| 17 | kadane | running best + reset |
| 18 | knapsack-0-1, lcs, edit-distance | 2D DP tables |
| 19 | dijkstra / bellman-ford | shortest paths, negative edges |
| 20 | prim / kruskal | MST, cut property, DSU |
| 21 | topo-sort | DAG ordering |
| 22 | segment-tree / fenwick-tree | range queries |

### Stage D — Master *(the hard problems)*
| # | Program | Core idea |
|:---:|:---|:---|
| 23 | kosaraju-scc, bridges-articulation | low-link thinking |
| 24 | n-queens, sudoku-solver | constraint pruning |
| 25 | knapsack-branch-bound, tsp-branch-and-bound | bounds + heuristics |
| 26 | kmp, z-algorithm, rabin-karp | preprocessing, rolling hash |
| 27 | tsp-bitmask-dp | exact exponential DP |
| 28 | max-flow, bipartite-matching | flow networks |
| 29 | convex-hull | geometric intuition |
| 30 | miller-rabin, reservoir-sampling | randomized algorithms |
| 31 | nim-game | game theory & XOR |
| 32 | function-pointers, generic-containers | master-level C |

---

## 🛠️ Getting Started

### Requirements
- A C compiler — **gcc** (MinGW on Windows, or any Linux/macOS gcc)

### Compile & run any program

```bash
# from the module folder
gcc -Wall -Wextra -o program array-basics.c
./program
```

### Try the classic first steps

```bash
# 1. The very first program
cd 01-fundamentals
gcc -Wall -Wextra -o basics array-basics.c && ./basics

# 2. Your first algorithm
cd ../03-searching
gcc -Wall -Wextra -o search binary-search.c && ./search

# 3. Your first data structure
cd ../05-stacks-queues-deques
gcc -Wall -Wextra -o stack stack.c && ./stack

# 4. Your first platform problem (LeetCode 146 - LRU Cache)
cd ../leetcode_hackerrank_problems
gcc -Wall -Wextra -o lru lru-cache-design.c && ./lru
```

> ⚠️ **Windows note:** use `gcc` from MinGW/MSYS2 or WSL. All programs are pure standard C — no platform-specific code.

---

## 📚 How Each Program Teaches

Every file follows the same lesson structure inside its header comment:

```
┌─────────────────────────────────────────────────────────────┐
│  PROBLEM DEFINITION                                        │
│  What this program solves                                  │
│                                                            │
│  REAL-LIFE EXAMPLE                                         │
│  The everyday situation this algorithm models              │
│                                                            │
│  DIAGRAM                                                   │
│  ASCII visual of the structure / process                   │
│                                                            │
│  PSEUDO CODE                                               │
│  The logic in plain language                               │
│                                                            │
│  SAMPLE INPUT & DRY RUN                                    │
│  Step-by-step trace of a small example                     │
│                                                            │
│  SPECIAL CASES                                             │
│  Edge cases where the result may surprise you              │
│                                                            │
│  TIPS                                                      │
│  Interview points, classic bugs, next steps                │
└─────────────────────────────────────────────────────────────┘
```

**Suggested study method:**
1. Read the header comment — understand *before* you run.
2. Run the program with your own inputs.
3. Trace one example by hand against the printed output.
4. Re-implement it from memory the next day.

---

## ⚡ Complexity Cheat Sheet

| Technique | Typical time | Space |
|:---|:---:|:---:|
| Linear scan / bubble sort | O(n) / O(n²) | O(1) |
| Binary search / balanced BST ops | O(log n) | O(1) |
| Merge / heap / quick sort | O(n log n) | O(n) / O(1) |
| Counting / radix sort | O(n + k) | O(n + k) |
| Hash table (average) | O(1) | O(n) |
| DP (knapsack, LCS, edit distance) | O(n·m) | O(n·m) |
| Dijkstra (array) | O(V²) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| TSP exact (bitmask DP) | O(n²·2ⁿ) | O(n·2ⁿ) |
| Max flow (Edmonds-Karp) | O(V·E²) | O(V²) |
| Bipartite matching (Kuhn) | O(V·E) | O(V) |
| Convex hull | O(n log n) | O(n) |
| Backtracking (worst) | exponential | O(n) |
| Sieve of Eratosthenes | O(n log log n) | O(n) |
| Miller-Rabin (k witnesses) | O(k log³ n) | O(1) |
| Boyer-Moore (average) | O(n/m) sub-linear | O(1) |
| Difference array (q batch updates) | O(n + q) | O(n) |
| Modular inverse (extended Euclid) | O(log m) | O(log m) |
| Largest histogram rectangle | O(n) | O(n) |
| Sparse table (build / query) | O(n log n) / O(1) | O(n log n) |
| Lazy segment tree (build / operation) | O(n) / O(log n) | O(n) |
| Weighted interval scheduling | O(n log n) | O(n) |
| 0–1 BFS (adjacency lists) | O(V + E) | O(V + E) |
| LCA binary lifting (build / query) | O(n log n) / O(log n) | O(n log n) |
| Manacher | O(n) | O(n) |
| Dinic max flow (general case) | O(V²·E) | O(V + E) |
| Point in polygon (validate / query) | O(n²) / O(n) | O(n) |
| Assignment bitmask DP | O(n·2ⁿ) | O(2ⁿ + n²) |

---

## 🧠 Beyond the Code

Some ideas cannot be written as a single program — but the programs above prepare you for them:

- **P vs NP** — after TSP, knapsack, and graph-coloring, you have felt the exponential wall firsthand
- **NP-completeness** — reductions between the problems you have coded
- **Approximation algorithms** — greedy on NP-hard problems (your greedy module is the seed)
- **Amortized analysis** — already live in union-find, splay trees, and queue-from-stacks
- **Sprague-Grundy theorem** — the math behind nim-game
- **Lower bounds** — why comparison sorting cannot beat O(n log n)

---

## 🤝 Contributing

Found a bug? Want to add a program? Contributions are welcome!

1. Fork the repository
2. Follow the existing lesson format (header comment structure above)
3. Keep programs **input-driven** and **self-documented**
4. Ensure `gcc -Wall -Wextra` compiles cleanly
5. Open a pull request

---

## 📄 License

This project is licensed under the **MIT License** — free to use, study, and teach.

---

<div align="center">

**Happy learning! 🎓**

*If this repository helped you, give it a ⭐ — it helps others find it too.*

</div>
