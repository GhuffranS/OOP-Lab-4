# SFML Tic-Tac-Toe Game

This project implements a classic Tic-Tac-Toe game using the SFML (Simple and Fast Multimedia Library) for graphics and user interaction. The game features a human player against an AI opponent with three difficulty levels: Easy, Medium, and Hard.

## Project Structure

The project consists of the following key components:

-   **`main.cpp`**: Contains the main game loop, SFML window setup, event handling, and rendering logic.
-   **`TicTacToeGame` Class**: Manages the game state, including the board, current player, game over status, winner, and AI logic.

## Dependencies

-   **SFML Library**: This project requires the SFML library to be installed and linked. You can download SFML from its official website ([https://www.sfml-dev.org/](https://www.sfml-dev.org/)) and follow the installation instructions for your operating system and compiler.

## How to Compile and Run

1.  **Ensure SFML is linked correctly in your project settings.** This typically involves specifying include directories and library directories, as well as linking against the necessary SFML libraries (e.g., `sfml-graphics`, `sfml-window`, `sfml-system`).
2.  **Save the provided code as `main.cpp`** (or any other `.cpp` file name).
3.  **Compile the code using a C++ compiler** (like g++) that is configured to work with SFML. For example, using g++ on Linux or macOS:

    ```bash
    g++ main.cpp -o tictactoe -lsfml-graphics -lsfml-window -lsfml-system
    ```

    On Windows, the linking flags might be slightly different (e.g., `-lsfml-graphics-s -lsfml-window-s -lsfml-system-s` for static linking, or without `-s` for dynamic linking, and potentially requiring `.lib` file names).

4.  **Run the executable:**

    ```bash
    ./tictactoe
    ```

    (On Windows, it might be `tictactoe.exe`).

## Game Features

-   **Graphical User Interface (GUI)**: Uses SFML to render the game board, X and O symbols, and buttons.
-   **Human vs. AI Gameplay**: Players can play against an AI opponent.
-   **Difficulty Levels**: Three AI difficulty levels are implemented:
    -   **Easy**: The AI makes random valid moves.
    -   **Medium**: The AI tries to win, blocks the opponent's winning moves, and otherwise makes random moves.
    -   **Hard**: The AI uses the Minimax algorithm to make optimal moves, ensuring it will never lose if the opponent plays optimally.
-   **Difficulty Selection**: Buttons are provided at the top of the window to select the desired difficulty level during the game. Changing the difficulty resets the game.
-   **Restart Button**: A "Restart" button allows players to easily start a new game.
-   **Game Over Detection**: The game automatically detects wins, losses, and draws.
-   **Status Display**: Text at the bottom of the window indicates the current player's turn or the game result.

## Code Structure Overview

### Enums

-   **`Player`**: Represents the players in the game (`NONE`, `HUMAN`, `AI`).
-   **`Difficulty`**: Defines the AI difficulty levels (`EASY`, `MEDIUM`, `HARD`).

### `TicTacToeGame` Class

-   **`board`**: A `std::vector<Player>` of size 9 representing the Tic-Tac-Toe board. Each element stores the player occupying that cell.
-   **`currentPlayer`**: A `Player` indicating whose turn it is.
-   **`gameOver`**: A `bool` flag indicating if the game has ended.
-   **`winner`**: A `Player` indicating the winner of the game (or `NONE` for a draw).
-   **`difficulty`**: The current AI difficulty level.
-   **`rng`**: A random number generator for AI moves.
-   **`reset()`**: Resets the game state to start a new game.
-   **`setDifficulty(Difficulty d)`**: Sets the AI difficulty level and resets the game.
-   **`isGameOver() const`**: Returns `true` if the game is over.
-   **`getWinner() const`**: Returns the winner of the game.
-   **`getCurrentPlayer() const`**: Returns the current player.
-   **`makeMove(int index)`**: Attempts to make a move at the given board index for the current player. Returns `true` if the move was successful.
-   **`aiMove()`**: Makes a move for the AI player based on the current difficulty level.
-   **`getCell(int index) const`**: Returns the player occupying the specified cell.
-   **`checkWin(Player p) const`**: Checks if the given player has won the game.
-   **`isBoardFull() const`**: Checks if the board is full (for a draw).
-   **`pickRandomMove()`**: Returns a random empty cell index.
-   **`pickMediumMove()`**: Implements the medium difficulty AI logic.
-   **`minimax(bool isMaximizing)`**: Implements the Minimax algorithm for the hard difficulty AI.

### Helper Functions

-   **`drawX(RenderWindow& window, Vector2f pos, float size, Color color)`**: Draws an "X" shape on the window.
-   **`drawO(RenderWindow& window, Vector2f center, float radius, Color color)`**: Draws an "O" shape on the window.

### `main()` Function

-   Initializes the SFML `RenderWindow`.
-   Creates an instance of the `TicTacToeGame` class.
-   Sets up the game board dimensions and drawing parameters.
-   Loads a font (Arial.ttf is expected in the same directory).
-   Creates buttons for difficulty selection and restarting the game.
-   Enters the main game loop:
    -   Handles SFML events (closing the window, mouse clicks).
    -   Processes mouse clicks on the board to make human moves.
    -   Triggers AI moves when it's the AI's turn and the game is not over.
    -   Clears the window.
    -   Draws the buttons, grid lines, X and O symbols on the board.
    -   Displays the game status (current turn or result).
    -   Displays the rendered frame.

## Potential Improvements

-   **Font Handling**: More robust font loading with error handling or using a default font if the specified one is not found.
-   **Visual Enhancements**: Add animations, more polished button styles, or visual feedback for player moves.
-   **Sound Effects**: Incorporate sound effects for moves, wins, losses, and draws.
-   **Game Settings**: Allow customization of player symbols or the order of play.
-   **Code Comments**: Add more detailed comments to explain specific parts of the code.
-   **Error Handling**: Implement more comprehensive error handling for various situations.
