# AVLTrees

## Overview  
This project implements **AVL trees** (self-balancing binary search trees) and related tree utilities in C++. The goal is to maintain balanced height on insertions and deletions, and to provide additional tree-based functions (e.g. checking equal path lengths) using recursion.  
It includes test suites to verify correctness under various scenarios.

## Repository Structure  

| File / Folder | Description |
|---|---|
| `avlbst.h` | Header file for your AVL tree class — holds declarations (node struct, insert, remove, balancing, rotations, etc.) |
| `bst.h` | Base binary search tree interface / utilities used by AVL (or helper BST functions) |
| `bst-test.cpp` | Test harness for BST / AVL tree operations |
| `equal-paths.cpp` / `equal-paths.h` | Functions and helpers to check whether all root-to-leaf paths in a tree have equal length |
| `equal-paths-test.cpp` | Tests for the equal-paths functionality under different tree shapes |
| `print_bst.h` | Utilities to print the tree (for debugging or visualization) |
| `Makefile` | Build configuration to compile tests and modules |
| `hw4_tests.tar.gz` | A bundled archive of test inputs / cases (for grading or verifying behavior) |
| `.gitignore` | Files/folders to ignore (e.g. build artifacts) |

## Features & Key Concepts  
- **AVL tree insertion and deletion**, including balancing via rotations (single and double)  
- Ensuring the **height-balance property**: for every node, height(left) and height(right) differ by at most 1  
- **Recursive traversal & utilities** like checking whether all root-to-leaf paths have equal length  
- Integration with **BST utilities** (search, minimum, maximum, traversal)  
- **Tree printing / visualization** to help debug and verify structure  
- Comprehensive testing via `bst-test.cpp` and `equal-paths-test.cpp` to cover edge cases (empty tree, skewed insertions, balancing, etc.)

## Dependencies & Requirements  
- C++ compiler supporting C++11 (or newer)  
- `make` or equivalent build tool  
- Standard C++ libraries only — no external dependencies  

## Build & Run Instructions  

From the project’s root directory:

```bash
make
