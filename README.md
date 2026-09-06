<div align="center">

# 🚀 Data Structures & Algorithms in C

### A Complete Learning Journey — From Zero to Mastery

**155 programs · 20 modules · 1 roadmap**

[![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Programs](https://img.shields.io/badge/Programs-155-2ea44f?style=for-the-badge)]()
[![Modules](https://img.shields.io/badge/Modules-20-8A2BE2?style=for-the-badge)]()
[![Compiler](https://img.shields.io/badge/Compiler-gcc%20%2DWall%20%2DWextra-FF6C37?style=for-the-badge&logo=gnu&logoColor=white)]()
[![License](https://img.shields.io/badge/License-MIT-4B8BBE?style=for-the-badge)]()

*Every program is input-driven, self-documented, and built to be understood.*

</div>

---

## 📖 Table of Contents

- [✨ Why This Repository](#-why-this-repository)
- [🎯 Who Is This For](#-who-is-this-for)
- [🧭 The Roadmap](#-the-roadmap)
- [📂 Repository Structure](#-repository-structure)
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

Every one of the **155 programs** is a complete, self-contained lesson that includes:

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

---

## 📂 Repository Structure

### 🟢 Level 1 — Foundations

| Module | Focus | Programs |
|:---:|:---|:---|
| **01-fundamentals** | C building blocks | array-basics, array-problems, subarray-techniques, number-checks, pattern-printing, swap-and-conversions, structure-basics, pointer-basics, pointer-operations, dynamic-memory, string-operations, bit-manipulation, matrix-operations, spiral-matrix |
| **02-recursion-and-mathematics** | Think recursively | factorial, recursion-techniques, tower-of-hanoi, string-permutations, pascal-triangle, josephus, gcd-euclid, fast-power, sieve-of-eratosthenes, math-techniques |

### 🟡 Level 2 — Core Techniques

| Module | Focus | Programs |
|:---:|:---|:---|
| **03-searching** | From O(n) to O(log n) | linear-search, binary-search, binary-search-variants, advanced-search-problems, jump-search, ternary-search, interpolation-search |
| **04-sorting** | Every major sort | bubble, selection, insertion, shell, merge, quick, heap, counting, bucket, radix, sorting-problems, structure-sort-quickselect |
| **05-stacks-queues-deques** | Linear ADTs & their power | stack, queue, deque, balanced-parentheses, infix-to-postfix, postfix-evaluation, stack-variants, queue-variants, next-greater-stock-span, sliding-window-maximum, deque-applications |
| **06-linked-lists** | Pointer surgery | singly, doubly, circular, linked-list-problems, polynomial-addition |

### 🟠 Level 3 — Hierarchical Data

| Module | Focus | Programs |
|:---:|:---|:---|
| **07-trees-and-heaps** | Trees, from plain to advanced | heap, binary-tree, bst, bst-problems, avl-tree, red-black-tree, b-tree, threaded-bst, trie, expression-tree, splay-tree, segment-tree, fenwick-tree, heap-applications, tree-problems-advanced |
| **08-hashing-and-union-find** | O(1) tricks & set magic | hash-table, hash-linear-probing, hash-problems, union-find, union-find-applications |

### 🔵 Level 4 — Design Paradigms

| Module | Focus | Programs |
|:---:|:---|:---|
| **09-greedy** | Local optimum → global optimum | activity-selection, coin-change-greedy, knapsack-fractional, job-sequencing, minimum-platforms, intervals-greedy, huffman-coding |
| **10-divide-and-conquer** | Split, solve, merge | min-max, closest-pair, inversions-median, strassen |
| **11-dynamic-programming** | Overlapping subproblems | fibonacci, climbing-stairs-house-robber, grid-dp, kadane, coin-change-dp, knapsack-0-1, lcs, lis, edit-distance, matrix-chain-multiplication, optimal-bst, rod-cutting, partition-equal-sum, palindromic-subsequence, egg-dropping, boolean-parenthesization, subset-sum-problems |

### 🔴 Level 5 — Advanced

| Module | Focus | Programs |
|:---:|:---|:---|
| **12-graphs** | The full graph toolkit | graph-problems, bfs, dfs, topo-sort, connected-components, dijkstra, bellman-ford, floyd-warshall, warshall, prim, kruskal, kosaraju-scc, bridges-articulation, bipartite-check, euler-path, grid-graph-problems |
| **13-backtracking** | Explore, choose, undo | subsets-combinations-word-search, n-queens, sum-of-subsets, rat-in-maze, knights-tour, sudoku-solver, hamiltonian-cycle, graph-coloring, tsp-backtracking |
| **14-branch-and-bound** | Search with pruning | knapsack-branch-bound, tsp-branch-and-bound, fifteen-puzzle-solver |
| **15-string-algorithms** | Pattern matching done right | naive-string-matching, kmp-string-matching, z-algorithm, rabin-karp, boyer-moore, string-problems, string-traversal-techniques, string-sorting-patterns |

### ⚫ Level 6 — Master

| Module | Focus | Programs |
|:---:|:---|:---|
| **16-network-flow** | Flow & matching | max-flow, bipartite-matching |
| **17-computational-geometry** | Geometry on a plane | convex-hull, line-segment-intersection |
| **18-randomized-algorithms** | Probability as a tool | miller-rabin, reservoir-sampling |
| **19-bitmask-dp-game-theory** | Subset DP & games | tsp-bitmask-dp, catalan-numbers, nim-game |
| **20-advanced-c-programming** | Master-level C | function-pointers, generic-containers, file-handling |

---

## 🚦 Learning Stages

A curated 36-step path through the repository — each step is one program.

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
