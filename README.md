# Terminal Tic-tac-toe with AI

A simple command-line implementation of Tic-tac-toe where you play against the computer. The AI opponent uses a mixed strategy of blocking player wins and pursuing its own victories.

## Features

- Terminal-based interface
- Random first player selection
- AI opponent with mixed strategy:
  - Will take winning moves when available
  - 70% chance to block player's winning moves
  - 30% chance to make aggressive moves instead of blocking
- Game logging system that records:
  - All moves made during the game
  - Final board state
  - Game outcome
  - Timestamp

## Requirements

- Windows 10
- GCC compiler (tested with MSYS2 MinGW-w64)

## Building

1. Clone the repository:
```bash
git clone https://github.com/badcircle/ttt.git
cd ttt
```

2. Compile with GCC:
```bash
gcc ttt.c -o ttt.exe
```

## Playing the Game

1. Run the executable:
```bash
./ttt.exe
```

2. The game will randomly select who goes first (player or CPU)

3. When it's your turn:
   - Enter your move as row and column numbers (1-3)
   - Separate numbers with a space
   - Example: `2 3` places your mark in row 2, column 3

4. Game ends when:
   - Player wins
   - CPU wins
   - Board is full (tie)

## Game Logs

The game automatically creates and appends to `tictactoe.log` in the working directory. Each game entry includes:
- Timestamp
- All moves made during the game
- Final board state
- Game outcome

## Code Structure

The code is organized to facilitate future networking capabilities:
- Game state is encapsulated in structures
- Clear separation between game logic and I/O operations
- Modular design for easy extension

## Future Plans

- Network multiplayer support
- Configurable AI difficulty
- GUI interface
- Game replay system

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.

## Contributing

Feel free to open issues or submit pull requests. All contributions are welcome!
