# Chess-app

**Version:** v0.2

My pet chess engine

---

## Features

- **Move Generator**: 200 million nodes per second.
- **Alpha-Beta Search**
- **Killer Moves**

## To Do

- **SSE for Ordering**
- **Improve Ordering**
- **PVS for Search**
- **Multithreading**
- **Zobrist Hashing**
- **Hash Tables**
- **Quiescence Search**
- **Better Static Evaluation**

---

## Build & Installation

- **CMake**: The engine uses CMake for build configuration.

### Installation

   ```bash
   git clone https://github.com/adidvar/chess-app
   cd chess-app
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build --config Release
   ./build/uci/uci
  ```
