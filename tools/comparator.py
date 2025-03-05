import subprocess
import time
import sys
import re
from datetime import datetime

class ChessEngine:
    def __init__(self, engine_path):
        self.path = engine_path
        self.process = None
        self.name = engine_path.split('/')[-1]
        print(f"[{self._timestamp()}] Initializing engine: {self.name}")
        self.start_engine()
        self.initialize_uci()

    def _timestamp(self):
        return datetime.now().strftime('%H:%M:%S')

    def start_engine(self):
        print(f"[{self._timestamp()}] Starting process for {self.name}...")
        self.process = subprocess.Popen(
            self.path,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        print(f"[{self._timestamp()}] Process started with PID: {self.process.pid}")

    def initialize_uci(self):
        """Initialize UCI engine with proper handshake"""
        print(f"[{self._timestamp()}] Initializing UCI protocol for {self.name}")

        # Send UCI command
        self.process.stdin.write("uci\n")
        self.process.stdin.flush()

        # Wait for uciok
        start_time = time.time()
        got_uciok = False
        while time.time() - start_time < 5:  # 5 second timeout
            line = self.process.stdout.readline()
            print(f"[{self._timestamp()}] {self.name} init response: {line.strip()}")
            if "uciok" in line:
                got_uciok = True
                break

        if not got_uciok:
            raise RuntimeError(f"Engine {self.name} did not respond to UCI init")

        # Send isready and wait for readyok
        print(f"[{self._timestamp()}] Sending isready to {self.name}")
        self.process.stdin.write("isready\n")
        self.process.stdin.flush()

        start_time = time.time()
        got_readyok = False
        while time.time() - start_time < 5:  # 5 second timeout
            line = self.process.stdout.readline()
            print(f"[{self._timestamp()}] {self.name} ready response: {line.strip()}")
            if "readyok" in line:
                got_readyok = True
                break

        if not got_readyok:
            raise RuntimeError(f"Engine {self.name} did not respond to isready")

        print(f"[{self._timestamp()}] {self.name} UCI initialization complete")

    def send_command(self, command, timeout=300):
        try:
            print(f"[{self._timestamp()}] Sending to {self.name}: {command.strip()}")
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()

            # For position commands, just return
            if command.startswith("position"):
                return ""

            # For perft commands, collect output
            output_lines = []
            start_time = time.time()

            while True:
                if time.time() - start_time > timeout:
                    raise TimeoutError(f"Engine {self.path} did not respond within {timeout} seconds")

                line = self.process.stdout.readline()
                if not line:
                    time.sleep(0.1)  # Short sleep to prevent CPU spinning
                    continue

                output_lines.append(line)

                # Check if we've reached the end of perft output
                if line.startswith("Nodes searched:") or "nodes" in line.lower():
                    break

            result = "".join(output_lines)
            print(f"[{self._timestamp()}] Received {len(output_lines)} lines from {self.name}")
            return result

        except Exception as e:
            print(f"[{self._timestamp()}] Error with {self.name}: {str(e)}")
            self.restart_engine()
            raise RuntimeError(f"Error communicating with engine {self.path}: {str(e)}")

    def restart_engine(self):
        print(f"[{self._timestamp()}] Restarting {self.name}...")
        self.quit()
        self.start_engine()
        self.initialize_uci()

    def quit(self):
        if self.process:
            print(f"[{self._timestamp()}] Shutting down {self.name}...")
            try:
                self.process.stdin.write("quit\n")
                self.process.stdin.flush()
                self.process.terminate()
                self.process.wait(timeout=5)
            except:
                print(f"[{self._timestamp()}] Force killing {self.name}")
                self.process.kill()
            finally:
                self.process = None

def parse_perft_output(output, engine_name):
    """Parse the perft output and return a dictionary of moves and their counts"""
    moves = {}
    move_count = 0
    for line in output.split('\n'):
        # Skip info strings and empty lines
        if line.startswith('info string') or not line.strip() or line.startswith('Nodes'):
            continue

        # Try to match move: number pattern
        match = re.match(r'([a-h][1-8][a-h][1-8](?:[pnbrqk])?)\s*:\s*(\d+)', line.strip())
        if match:
            move, count = match.groups()
            moves[move] = int(count)
            move_count += 1

    print(f"[{datetime.now().strftime('%H:%M:%S')}] Parsed {move_count} moves from {engine_name}")
    return moves

def compare_perft_at_position(engine1, engine2, position, depth, move_history=None):
    """
    Recursively compare perft results at a given position
    """
    if move_history is None:
        move_history = []

    current_depth = len(move_history)
    indent = "  " * current_depth
    print(f"\n{indent}[{datetime.now().strftime('%H:%M:%S')}] Analyzing depth {depth} after moves: {' '.join(move_history) or 'startpos'}")

    # Set up position
    position_cmd = f"position {position}"
    if move_history:
        position_cmd += f" moves {' '.join(move_history)}"

    # Get perft results from both engines
    try:
        # Send position to both engines
        engine1.send_command(position_cmd)
        engine2.send_command(position_cmd)

        print(f"{indent}Running perft {depth} on both engines...")
        output1 = engine1.send_command(f"go perft {depth}")
        output2 = engine2.send_command(f"go perft {depth}")

        moves1 = parse_perft_output(output1, engine1.name)
        moves2 = parse_perft_output(output2, engine2.name)

        # Compare move counts
        differences = []
        all_moves = set(moves1.keys()) | set(moves2.keys())

        for move in all_moves:
            count1 = moves1.get(move, 0)
            count2 = moves2.get(move, 0)

            if count1 != count2:
                differences.append((move, count1, count2))

        print(f"{indent}Found {len(differences)} differences at current position")

        # If differences found and depth > 1, recursively check each differing move
        if differences and depth > 1:
            print(f"\n{indent}Position after moves: {' '.join(move_history) or 'startpos'}")
            print(f"{indent}{'Move':<10} {engine1.name:<15} {engine2.name:<15} Difference")
            print(f"{indent}{'-' * 50}")

            for move, count1, count2 in sorted(differences):
                diff = count2 - count1
                print(f"{indent}{move:<10} {count1:<15} {count2:<15} {diff:+}")

                # Recursively analyze this move with depth-1
                new_history = move_history + [move]
                print(f"\n{indent}Recursing into position after {move}...")
                compare_perft_at_position(engine1, engine2, position, depth-1, new_history)

        return differences

    except Exception as e:
        print(f"{indent}Error at position {' '.join(move_history)}: {str(e)}")
        return []

def main():
    if len(sys.argv) != 5:
        print("Usage: python script.py <engine1_path> <engine2_path> <depth> <FEN>")
        print("Example: python script.py ./myengine ./stockfish 7 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1'")
        return

    engine1_path = sys.argv[1]
    engine2_path = sys.argv[2]
    depth = int(sys.argv[3])
    fen = sys.argv[4]

    print(f"[{datetime.now().strftime('%H:%M:%S')}] Starting perft comparison")
    print(f"Engine 1: {engine1_path}")
    print(f"Engine 2: {engine2_path}")
    print(f"Depth: {depth}")
    print(f"FEN: {fen}")

    try:
        engine1 = ChessEngine(engine1_path)
        engine2 = ChessEngine(engine2_path)

        print(f"\nStarting recursive perft analysis to depth {depth}...")
        differences = compare_perft_at_position(engine1, engine2, fen, depth)

        if not differences:
            print("\nNo differences found between engines at the given position!")

    except Exception as e:
        print(f"Error: {str(e)}")
        sys.exit(1)
    finally:
        if 'engine1' in locals(): engine1.quit()
        if 'engine2' in locals(): engine2.quit()

if __name__ == "__main__":
    main()
