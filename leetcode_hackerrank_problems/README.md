# 🎯 LeetCode & HackerRank Problems — C Solutions

Each `.c` file in this folder is a complete, self-contained lesson that solves one
or more **numbered platform problems**. The header of every file begins with a
`PLATFORM PROBLEMS:` block naming the exact LeetCode numbers and HackerRank
titles it covers, followed by the usual lesson structure (problem definition,
real-life example, diagram, pseudo code, dry run, special cases, complexity, tips).

## Compile & run

```bash
# from this folder
gcc -Wall -Wextra -o lru lru-cache-design.c && ./lru
```

Every program is **input-driven**: it prints a worked sample, then reads its own
test case from `stdin`. All files also compile warning-free with the strict set:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o out <file>.c
```

## Index by platform problem

### LeetCode (by problem number)

| # | Problem | File |
|:---:|:---|:---|
| 1 | Two Sum | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| 3 | Longest Substring Without Repeating Characters | [sliding-window.c](sliding-window.c) |
| 4 | Median of Two Sorted Arrays | [median-two-sorted.c](median-two-sorted.c) |
| 7 | Reverse Integer | [digit-math.c](digit-math.c) |
| 8 | String to Integer (atoi) | [string-interview-problems.c](string-interview-problems.c) |
| 9 | Palindrome Number | [digit-math.c](digit-math.c) |
| 14 | Longest Common Prefix | [string-interview-problems.c](string-interview-problems.c) |
| 17 | Letter Combinations of a Phone Number | [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| 20 | Valid Parentheses | [string-interview-problems.c](string-interview-problems.c) |
| 21 | Merge Two Sorted Lists | [median-two-sorted.c](median-two-sorted.c) |
| 22 | Generate Parentheses | [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| 39 | Combination Sum | [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| 41 | First Missing Positive | [array-hard-interview-problems.c](array-hard-interview-problems.c) |
| 42 | Trapping Rain Water | [monotonic-stack.c](monotonic-stack.c) |
| 44 | Wildcard Matching | [dp-interview-problems.c](dp-interview-problems.c) |
| 46 | Permutations | [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| 49 | Group Anagrams | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| 56 | Merge Intervals | [interval-problems.c](interval-problems.c) |
| 57 | Insert Interval | [interval-problems.c](interval-problems.c) |
| 62 | Unique Paths | [combinatorics.c](combinatorics.c) |
| 69 | Sqrt(x) | [binary-search-on-answer.c](binary-search-on-answer.c) |
| 71 | Simplify Path | [expression-evaluation-interview.c](expression-evaluation-interview.c) |
| 74 | Search a 2D Matrix | [matrix-search.c](matrix-search.c) |
| 76 | Minimum Window Substring | [sliding-window.c](sliding-window.c) |
| 78 | Subsets | [bit-tricks-interview.c](bit-tricks-interview.c) · [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| 84 | Largest Rectangle in Histogram | [monotonic-stack.c](monotonic-stack.c) |
| 85 | Maximal Rectangle | [dp-hard-interval-and-grid.c](dp-hard-interval-and-grid.c) |
| 88 | Merge Sorted Array | [two-pointers.c](two-pointers.c) |
| 91 | Decode Ways | [dp-interview-problems.c](dp-interview-problems.c) |
| 96 | Unique Binary Search Trees | [combinatorics.c](combinatorics.c) |
| 98 | Validate Binary Search Tree | [bst-interview-problems.c](bst-interview-problems.c) |
| 102 | Binary Tree Level Order Traversal | [tree-interview-problems.c](tree-interview-problems.c) |
| 104 | Maximum Depth of Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| 110 | Balanced Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| 112 | Path Sum | [tree-interview-problems.c](tree-interview-problems.c) |
| 118 | Pascal's Triangle | [combinatorics.c](combinatorics.c) |
| 121 · 122 | Best Time to Buy and Sell Stock I / II | [stock-dp.c](stock-dp.c) |
| 123 · 188 | Stock III / Stock IV | [stock-dp.c](stock-dp.c) |
| 124 | Binary Tree Maximum Path Sum | [tree-dp-hard.c](tree-dp-hard.c) |
| 125 | Valid Palindrome | [two-pointers.c](two-pointers.c) · [string-interview-problems.c](string-interview-problems.c) |
| 126 · 127 | Word Ladder II / I | [word-ladder-bfs.c](word-ladder-bfs.c) |
| 128 | Longest Consecutive Sequence | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| 136 · 137 | Single Number I / II | [bit-tricks-interview.c](bit-tricks-interview.c) |
| 139 | Word Break | [dp-interview-problems.c](dp-interview-problems.c) |
| 146 | LRU Cache | [lru-cache-design.c](lru-cache-design.c) |
| 151 | Reverse Words in a String | [string-interview-problems.c](string-interview-problems.c) |
| 152 | Maximum Product Subarray | [dp-hard-interval-and-grid.c](dp-hard-interval-and-grid.c) |
| 167 | Two Sum II | [two-pointers.c](two-pointers.c) |
| 191 | Number of 1 Bits | [bit-tricks-interview.c](bit-tricks-interview.c) |
| 200 | Number of Islands | [grid-bfs-paths.c](grid-bfs-paths.c) |
| 207 · 210 | Course Schedule I / II | [course-schedule-topo.c](course-schedule-topo.c) |
| 209 | Minimum Size Subarray Sum | [sliding-window.c](sliding-window.c) |
| 217 | Contains Duplicate | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| 218 | The Skyline Problem | [hard-greedy-and-sweep.c](hard-greedy-and-sweep.c) |
| 221 | Maximal Square | [dp-hard-interval-and-grid.c](dp-hard-interval-and-grid.c) |
| 224 | Basic Calculator | [expression-evaluation-interview.c](expression-evaluation-interview.c) |
| 226 | Invert Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| 227 | Basic Calculator II | [expression-evaluation-interview.c](expression-evaluation-interview.c) |
| 229 | Majority Element II | [array-hard-interview-problems.c](array-hard-interview-problems.c) |
| 230 | Kth Smallest Element in a BST | [bst-interview-problems.c](bst-interview-problems.c) |
| 231 | Power of Two | [bit-tricks-interview.c](bit-tricks-interview.c) |
| 236 | Lowest Common Ancestor of a Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| 238 | Product of Array Except Self | [array-hard-interview-problems.c](array-hard-interview-problems.c) |
| 240 | Search a 2D Matrix II | [matrix-search.c](matrix-search.c) |
| 252 · 253 | Meeting Rooms I / II | [interval-problems.c](interval-problems.c) |
| 258 | Add Digits | [digit-math.c](digit-math.c) |
| 269 | Alien Dictionary | [course-schedule-topo.c](course-schedule-topo.c) |
| 279 | Perfect Squares | [dp-interview-problems.c](dp-interview-problems.c) |
| 300 | Longest Increasing Subsequence | [dp-interview-problems.c](dp-interview-problems.c) |
| 304 | Range Sum Query 2D | [prefix-sum-2d.c](prefix-sum-2d.c) |
| 309 · 714 | Stock with Cooldown / Fee | [stock-dp.c](stock-dp.c) |
| 312 | Burst Balloons | [dp-hard-interval-and-grid.c](dp-hard-interval-and-grid.c) |
| 315 | Count of Smaller Numbers After Self | [divide-conquer-hard.c](divide-conquer-hard.c) |
| 316 | Remove Duplicate Letters | [hard-greedy-and-sweep.c](hard-greedy-and-sweep.c) |
| 322 | Coin Change | [dp-interview-problems.c](dp-interview-problems.c) |
| 337 | House Robber III | [tree-dp-hard.c](tree-dp-hard.c) |
| 338 | Counting Bits | [bit-tricks-interview.c](bit-tricks-interview.c) |
| 344 | Reverse String | [two-pointers.c](two-pointers.c) |
| 378 | Kth Smallest in a Sorted Matrix | [matrix-search.c](matrix-search.c) |
| 394 | Decode String | [expression-evaluation-interview.c](expression-evaluation-interview.c) |
| 402 | Remove K Digits | [hard-greedy-and-sweep.c](hard-greedy-and-sweep.c) |
| 410 | Split Array Largest Sum | [binary-search-on-answer.c](binary-search-on-answer.c) |
| 424 | Longest Repeating Character Replacement | [sliding-window.c](sliding-window.c) |
| 433 | Minimum Genetic Mutation | [word-ladder-bfs.c](word-ladder-bfs.c) |
| 435 | Non-overlapping Intervals | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| 443 | String Compression | [string-interview-problems.c](string-interview-problems.c) |
| 448 | Find All Numbers Disappeared in an Array | [array-hard-interview-problems.c](array-hard-interview-problems.c) |
| 450 | Delete Node in a BST | [bst-interview-problems.c](bst-interview-problems.c) |
| 460 | LFU Cache | [lru-cache-design.c](lru-cache-design.c) |
| 493 | Reverse Pairs | [divide-conquer-hard.c](divide-conquer-hard.c) |
| 496 | Next Greater Element I | [monotonic-stack.c](monotonic-stack.c) |
| 542 | 01 Matrix | [grid-bfs-paths.c](grid-bfs-paths.c) |
| 543 | Diameter of Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| 547 | Number of Provinces | [union-find-interview-problems.c](union-find-interview-problems.c) |
| 560 | Subarray Sum Equals K | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| 567 · 643 | Permutation in String / Maximum Average Subarray | [sliding-window.c](sliding-window.c) |
| 621 | Task Scheduler | [hard-greedy-and-sweep.c](hard-greedy-and-sweep.c) |
| 684 | Redundant Connection | [union-find-interview-problems.c](union-find-interview-problems.c) |
| 700 · 701 | Search / Insert in a BST | [bst-interview-problems.c](bst-interview-problems.c) |
| 706 · 707 | Design HashMap / Linked List | [lru-cache-design.c](lru-cache-design.c) |
| 721 | Accounts Merge | [union-find-interview-problems.c](union-find-interview-problems.c) |
| 739 | Daily Temperatures | [monotonic-stack.c](monotonic-stack.c) |
| 802 | Find Eventual Safe States | [course-schedule-topo.c](course-schedule-topo.c) |
| 875 | Koko Eating Bananas | [binary-search-on-answer.c](binary-search-on-answer.c) |
| 901 | Online Stock Span | [monotonic-stack.c](monotonic-stack.c) |
| 912 | Sort an Array | [divide-conquer-hard.c](divide-conquer-hard.c) |
| 938 | Range Sum of BST | [bst-interview-problems.c](bst-interview-problems.c) |
| 959 | Regions Cut By Slashes | [union-find-interview-problems.c](union-find-interview-problems.c) |
| 968 | Binary Tree Cameras | [tree-dp-hard.c](tree-dp-hard.c) |
| 977 | Squares of a Sorted Array | [two-pointers.c](two-pointers.c) |
| 994 | Rotting Oranges | [grid-bfs-paths.c](grid-bfs-paths.c) |
| 1011 | Capacity to Ship Packages Within D Days | [binary-search-on-answer.c](binary-search-on-answer.c) |
| 1029 | Two City Scheduling | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| 1091 | Shortest Path in a Binary Matrix | [grid-bfs-paths.c](grid-bfs-paths.c) |
| 1288 | Remove Covered Intervals | [interval-problems.c](interval-problems.c) |
| 1314 | Matrix Block Sum | [prefix-sum-2d.c](prefix-sum-2d.c) |
| 1710 | Maximum Units on a Truck | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| 1926 | Nearest Exit from Entrance in Maze | [grid-bfs-paths.c](grid-bfs-paths.c) |

### HackerRank

| Problem | File |
|:---|:---|
| 2D Array – DS (maximum hourglass sum) | [prefix-sum-2d.c](prefix-sum-2d.c) |
| New Year Chaos | [hackerrank-tricky-classics.c](hackerrank-tricky-classics.c) |
| Minimum Swaps 2 | [hackerrank-tricky-classics.c](hackerrank-tricky-classics.c) |
| Maximum Subarray Sum | [hackerrank-tricky-classics.c](hackerrank-tricky-classics.c) |
| Simple Text Editor (undo stack) | [hackerrank-tricky-classics.c](hackerrank-tricky-classics.c) |
| Merge Sort: Counting Inversions | [divide-conquer-hard.c](divide-conquer-hard.c) |
| Balanced Brackets (nesting stack) | [expression-evaluation-interview.c](expression-evaluation-interview.c) |
| Big Sorting | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| Mark and Toys | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| Maximum Perimeter Triangle | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| Priyanka and Toys | [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) |
| Largest Rectangle | [monotonic-stack.c](monotonic-stack.c) |
| Cutting Paper Squares (feasibility searches) | [binary-search-on-answer.c](binary-search-on-answer.c) |
| Find the Median | [median-two-sorted.c](median-two-sorted.c) |
| nCr / Handshake | [combinatorics.c](combinatorics.c) |
| Recursive Digit Sum | [digit-math.c](digit-math.c) |
| Matrix (count below threshold) | [matrix-search.c](matrix-search.c) |
| Print in Reverse (traversal drills) | [two-pointers.c](two-pointers.c) |
| Substring Diff | [sliding-window.c](sliding-window.c) |
| Cycle Detection (design drills) | [lru-cache-design.c](lru-cache-design.c) |
| Tree: Height of a Binary Tree | [tree-interview-problems.c](tree-interview-problems.c) |
| BST: Insertion / Lowest Common Ancestor | [bst-interview-problems.c](bst-interview-problems.c) |
| Pairs / Count Triplets | [hashmap-interview-problems.c](hashmap-interview-problems.c) |
| Merging Communities · Kundu and Tree | [union-find-interview-problems.c](union-find-interview-problems.c) |
| Stock Maximize | [stock-dp.c](stock-dp.c) |
| The Maximum Subarray / Coin Change | [dp-interview-problems.c](dp-interview-problems.c) |
| Shortest Reach in a Graph | [grid-bfs-paths.c](grid-bfs-paths.c) · [word-ladder-bfs.c](word-ladder-bfs.c) |
| Topological Sort | [course-schedule-topo.c](course-schedule-topo.c) |
| N-Queens family recursion drills | [backtracking-interview-problems.c](backtracking-interview-problems.c) |
| Super Reduced String | [string-interview-problems.c](string-interview-problems.c) |
| Lonely Integer · Maximizing XOR | [bit-tricks-interview.c](bit-tricks-interview.c) |

### Files at a glance

| File | Focus |
|:---|:---|
| [two-pointers.c](two-pointers.c) | Pair sum, reverse, palindrome, merge in one pass |
| [prefix-sum-2d.c](prefix-sum-2d.c) | O(1) submatrix sums after a 2D prefix build |
| [digit-math.c](digit-math.c) | Digits, digital root, palindrome, Armstrong checks |
| [combinatorics.c](combinatorics.c) | Overflow-safe nCr/nPr, Catalan, grid paths |
| [binary-search-on-answer.c](binary-search-on-answer.c) | Koko, ship capacity, split array, sqrt |
| [matrix-search.c](matrix-search.c) | Staircase search + kth smallest via counting |
| [median-two-sorted.c](median-two-sorted.c) | Partition binary search with exact boundaries |
| [interval-problems.c](interval-problems.c) | Merge, insert, meeting rooms, covered counting |
| [hackerrank-sorting-classics.c](hackerrank-sorting-classics.c) | Six sort-then-greedy HackerRank staples |
| [monotonic-stack.c](monotonic-stack.c) | Next greater, temperatures, rectangle, rain water |
| [sliding-window.c](sliding-window.c) | Fixed and variable windows in one amortised pass |
| [lru-cache-design.c](lru-cache-design.c) | O(1) get/put with list + map (self-tested) |
| [tree-interview-problems.c](tree-interview-problems.c) | Depth, balance, diameter, invert, LCA, paths |
| [bst-interview-problems.c](bst-interview-problems.c) | Validate, kth, floor/ceil, range sum, delete |
| [hashmap-interview-problems.c](hashmap-interview-problems.c) | Two sum, runs, anagram grouping, subarray sums |
| [union-find-interview-problems.c](union-find-interview-problems.c) | Provinces, redundant edge, accounts merge |
| [stock-dp.c](stock-dp.c) | Every buy/sell variant as state transitions |
| [dp-interview-problems.c](dp-interview-problems.c) | Decode ways, word break, coins, LIS, wildcards |
| [grid-bfs-paths.c](grid-bfs-paths.c) | Shortest path, islands, multi-source BFS |
| [course-schedule-topo.c](course-schedule-topo.c) | Kahn's algorithm with cycle reporting |
| [word-ladder-bfs.c](word-ladder-bfs.c) | BFS over an implicit one-letter-change word graph |
| [backtracking-interview-problems.c](backtracking-interview-problems.c) | Permutations, combination sum, keypad, parens |
| [string-interview-problems.c](string-interview-problems.c) | atoi, parentheses, run-length, prefix, reverse |
| [bit-tricks-interview.c](bit-tricks-interview.c) | XOR pairing, popcount, counting bits, subsets |
| [array-hard-interview-problems.c](array-hard-interview-problems.c) | Place-at-index, Boyer-Moore II, prefix/suffix products |
| [divide-conquer-hard.c](divide-conquer-hard.c) | Inversions, reverse pairs, smaller-after-self in one merge |
| [dp-hard-interval-and-grid.c](dp-hard-interval-and-grid.c) | Burst balloons, maximal square/rectangle, max product |
| [tree-dp-hard.c](tree-dp-hard.c) | Tree DP returning tuples: robber, path sum, cameras |
| [hard-greedy-and-sweep.c](hard-greedy-and-sweep.c) | Skyline sweep, task scheduler gaps, monotone digit stack |
| [expression-evaluation-interview.c](expression-evaluation-interview.c) | Recursive-descent calculator, decode string, simplify path |
| [hackerrank-tricky-classics.c](hackerrank-tricky-classics.c) | New Year Chaos, minimum swaps, Kadane, text editor undo |
