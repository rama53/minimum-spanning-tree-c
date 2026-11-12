# Known issues

The original source and input are preserved unchanged. Findings below come from a modern GCC C11 syntax check, source inspection, and an independent dataset connectivity check.

## Compilation

- `gets` is unsafe and was removed from C11.
- `kruskals`, `leftChild`, and `rightChild` are called before declarations are provided.
- `search_name` has a bare `return` despite returning a pointer.
- Heap allocation, sentinel initialization, and swapping mix adjacency-node pointers (`ptr`, `struct Node *`) with edge pointers (`edge`, `struct node *`). Modern GCC reports incompatible pointer types.
- `printGraph` passes an extra argument to `printf`; Kruskal’s cleanup also has an unused local variable.

## Dataset and loading

- `cities.txt` contains 165 cities, 299 rows, and 186 distinct undirected city pairs. The graph has two connected components, so a full MST does not exist.
- Duplicate/reversed rows increment `graphSize` even when the adjacency insertion is skipped. Printed graph size counts rows rather than distinct roads.
- The loader retains the first weight for an existing connection and ignores later weights for that pair.
- The assignment minimum of 200 edges is not met when counting distinct undirected city pairs.
- Fixed-size buffers and unbounded string copies can overflow; missing input fields may leave uninitialized buffers in use.

## Algorithms and comparison

- The comparison menu labels the slower algorithm as faster: when Kruskal’s time is greater, it says Kruskal’s is faster, and vice versa.
- The comparison shows stored timings but does not compare total costs as required. It does not ensure both algorithms have run; global timings initially equal zero.
- Timing scopes differ and include work beyond selecting MST edges, including some printing or cleanup. Single-run measurements are not a controlled benchmark.
- Kruskal’s can be invoked before loading a graph, resulting in zero or negative variable-length array dimensions.
- Heap capacity checking permits an insertion one slot beyond the allocated array boundary if the advertised capacity is reached.
- Heap creation is not reliably checked by its callers. City insertion can fall through without returning a value at capacity, and its collision-probing loop has no termination bound.
- Memory cleanup is incomplete: the heap sentinel, edges drained during Kruskal cleanup, remaining Prim heap edges, and the graph/city allocations can leak.
- Interactive numeric input is not checked robustly, and final graph memory is not released on exit.

Runtime behavior has not been verified, and the repository does not claim that every assignment requirement is satisfied.
