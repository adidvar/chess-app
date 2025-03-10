# Chess-app

**Version:** v0.2

My pet UCI-compatible chess engine

## Features

- **Move Generator**: Extremely fast
   - **Magic Bitboards**: Uses classical magic bitboard lookup tables implementation for sliding pieces.
   - **Intrinsics**: Utilizes compiler intrinsics for `bsr` (bit scan reverse) and `popcount` (population count).
   - **Hashed Positions for Kings and Knights**: Fast move generation for kings and knights using lookup tables.
   - **Branchless**: Uses a lookup table of generators for different bitboard flags (rook, color, en passant).
   - **Speed**: **200 million** nodes per second in perft.

- **Alpha-Beta Search**: Implements alpha-beta pruning to reduce the number of nodes evaluated.
- **Killer Moves**: Prioritizes strong candidate moves to improve search efficiency.
- **Move Ordering**: Uses heuristics to improve search speed by evaluating the most promising moves first.


## To Restore (from old code)

- **Principal Variation Search (PVS)**: Optimizes the search by assuming that the best move is found early.
- **Zobrist Hashing**: Efficiently stores game states to avoid redundant calculations.
- **Hash Tables**: Implements transposition tables to speed up position evaluations.
- **Quiescence Search**: Reduces the horizon effect by searching only relevant moves in critical positions.

## To Do

- Implement SSE for move ordering.
- Improve move ordering.
- Add multithreading support.

## Build & Installation

### Requirements

- CMake 3.22.1 or later
- C++20-compatible compiler

### Installation

```sh
git clone https://github.com/adidvar/chess-app
cd chess-app
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/uci/uci
```

After building, the engine will be ready to run as a UCI-compatible chess engine.

## Contributing

Contributions are welcome! If you find a bug, have an idea for an improvement, or want to add a new feature, feel free to open an issue or submit a pull request. Your contributions help improve the project!
