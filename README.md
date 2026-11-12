# Minimum Spanning Tree in C

A console application by **Rama Anabtawi** for **COMP2421 — Data Structures, Project 3 (Spring 2024/2025)**. The project models road connections between Palestinian cities and towns and implements **Prim’s** and **Kruskal’s** algorithms to explore minimum-cost road networks.

## Features

- Load the weighted, undirected graph from `cities.txt` and display its adjacency lists, order, and size.
- Run Prim’s algorithm from a chosen source city.
- Run Kruskal’s algorithm using a minimum heap and disjoint-set structure.
- Display selected edges, total cost, and CPU time measured with `clock()`.
- Compare the stored algorithm timings through a menu option.

The original code has compilation and behavior issues. See [known issues](docs/KNOWN_ISSUES.md) before use.

## Implementation

Cities are stored in a hash table with 1,009 slots. Linked adjacency lists represent undirected roads, and a binary min-heap selects edges by distance. Prim’s algorithm tracks visited cities; Kruskal’s algorithm detects cycles using parent indices and path compression. The implementation also scans city arrays when mapping Kruskal edge endpoints, so textbook optimized complexity bounds do not directly describe its performance.

## Repository contents

| Path | Description |
| --- | --- |
| `P3_1230338_RamaAnabtawi_1.c` | Original C implementation, preserved unchanged |
| `cities.txt` | Original road-network input |
| `docs/project-instructions.pdf` | Original assignment PDF |
| `docs/KNOWN_ISSUES.md` | Compilation, dataset, and behavior limitations |

Compiled executables, object files, and local IDE state are excluded.

## Build and run

A C compiler such as GCC is required. The source uses the standard C headers for input, memory, strings, and timing; no external library is required.

**The original source currently fails modern GCC C11 compilation.** Once the documented issues are corrected, the intended commands are:

```sh
gcc -std=c11 -Wall -Wextra P3_1230338_RamaAnabtawi_1.c -o mst
./mst
```

On Windows with MinGW, build with `-o mst.exe` and run `.\mst.exe`.

Run from the repository root so that `cities.txt` is available. The menu offers:

1. Load cities.
2. Apply Prim’s algorithm (enter an exact, case-sensitive city name, such as `Jenin`).
3. Apply Kruskal’s algorithm.
4. Compare the stored execution times.
5. Exit.

Load the graph before running either algorithm. Run both algorithms before selecting the comparison option. The comparison output has known defects described separately. No output report file is generated.

## Input format and supplied data

Each line contains two city names and an integer distance in kilometers, separated by `#`:

```text
Jenin#Nablus#40
Nablus#Ramallah#35
Ramallah#Jerusalem#15
```

Roads are treated as undirected. The loader keeps the first connection encountered for a city pair rather than adding parallel connections.

The supplied file contains **165 distinct city names**, **299 nonempty rows**, and **186 distinct undirected city pairs**. An independent connectivity check found **two connected components**. Therefore, a single spanning tree covering all cities does not exist for this dataset. Prim’s algorithm can explore its starting component; Kruskal’s can produce a forest. The program reports an incomplete result when it selects fewer than `order - 1` edges.

The assignment requires at least 50 vertices and 200 edges. Although the input has 299 rows, duplicate and reversed connections mean its distinct undirected edge count is below 200. Original data has been retained unchanged.

## Assignment and validation

The [assignment PDF](docs/project-instructions.pdf) describes the road-network problem, required menu, dataset requirements, and original submission rules.

Validation included a modern GCC syntax check and an independent dataset check of city count, distinct undirected connections, and connectivity. The command `gcc -std=c11 -Wall -Wextra -fsyntax-only P3_1230338_RamaAnabtawi_1.c` failed. Runtime correctness and algorithm timings have not been verified. Documentation was added for publication without changing the C source or input file.

## Attribution and license

Original C implementation: Rama Anabtawi. The assignment PDF retains its original attribution. No open-source license is granted by this repository.
