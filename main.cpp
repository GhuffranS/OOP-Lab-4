#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

using namespace sf;

enum Player { NONE = 0, HUMAN = 1, AI = 2 };
enum Difficulty { EASY = 0, MEDIUM, HARD };

class TicTacToeGame {
public:
    TicTacToeGame() {
        board.resize(9, NONE);
        currentPlayer = HUMAN;
        gameOver = false;
        winner = NONE;
        difficulty = MEDIUM;
        rng.seed(std::random_device{}());
    }

    void reset() {
        std::fill(board.begin(), board.end(), NONE);
        currentPlayer = HUMAN;
        gameOver = false;
        winner = NONE;
    }

    void setDifficulty(Difficulty d) {
        difficulty = d;
        reset();
    }

    bool isGameOver() const {
        return gameOver;
    }

    Player getWinner() const {
        return winner;
    }

    Player getCurrentPlayer() const {
        return currentPlayer;
    }

    bool makeMove(int index) {
        if (index < 0 || index >= 9) return false;
        if (board[index] != NONE || gameOver) return false;

        board[index] = currentPlayer;

        if (checkWin(currentPlayer)) {
            gameOver = true;
            winner = currentPlayer;
        } else if (isBoardFull()) {
            gameOver = true;
            winner = NONE; // draw
        } else {
            currentPlayer = (currentPlayer == HUMAN) ? AI : HUMAN;
        }
        return true;
    }

    void aiMove() {
        if (gameOver || currentPlayer != AI) return;

        int moveIndex = -1;

        if (difficulty == EASY) {
            moveIndex = pickRandomMove();
        } else if (difficulty == MEDIUM) {
            moveIndex = pickMediumMove();
        } else { // HARD
            int score = std::numeric_limits<int>::min();
            for (int i = 0; i < 9; i++) {
                if (board[i] == NONE) {
                    board[i] = AI;
                    int moveScore = minimax(false);
                    board[i] = NONE;
                    if (moveScore > score) {
                        score = moveScore;
                        moveIndex = i;
                    }
                }
            }
        }

        makeMove(moveIndex);
    }

    Player getCell(int index) const {
        if (index < 0 || index >= 9) return NONE;
        return board[index];
    }

private:
    std::vector<Player> board;
    Player currentPlayer;
    bool gameOver;
    Player winner;
    Difficulty difficulty;

    std::mt19937 rng;

    bool checkWin(Player p) const {
        static const int winPositions[8][3] = {
            {0,1,2},{3,4,5},{6,7,8}, // rows
            {0,3,6},{1,4,7},{2,5,8}, // columns
            {0,4,8},{2,4,6}          // diagonals
        };
        for (auto& line : winPositions) {
            if (board[line[0]] == p && board[line[1]] == p && board[line[2]] == p)
                return true;
        }
        return false;
    }

    bool isBoardFull() const {
        return std::none_of(board.begin(), board.end(), [](Player p){return p == NONE;});
    }

    int pickRandomMove() {
        std::vector<int> emptyCells;
        for(int i = 0; i < 9; i++) {
            if(board[i] == NONE) emptyCells.push_back(i);
        }
        std::uniform_int_distribution<int> dist(0, emptyCells.size() - 1);
        return emptyCells[dist(rng)];
    }

    // Medium difficulty: Try to win, block opponent winning move, else random
    int pickMediumMove() {
        // Try to win
        for(int i=0; i<9; i++) {
            if(board[i] == NONE) {
                board[i] = AI;
                if(checkWin(AI)) {
                    board[i] = NONE;
                    return i;
                }
                board[i] = NONE;
            }
        }
        // Block human win
        for(int i=0; i<9; i++) {
            if(board[i] == NONE) {
                board[i] = HUMAN;
                if(checkWin(HUMAN)) {
                    board[i] = NONE;
                    return i;
                }
                board[i] = NONE;
            }
        }
        // Else random
        return pickRandomMove();
    }

    int minimax(bool isMaximizing) {
        if (checkWin(AI)) return 10;
        if (checkWin(HUMAN)) return -10;
        if (isBoardFull()) return 0;

        if (isMaximizing) {
            int bestScore = std::numeric_limits<int>::min();
            for (int i = 0; i < 9; i++) {
                if (board[i] == NONE) {
                    board[i] = AI;
                    int score = minimax(false);
                    board[i] = NONE;
                    bestScore = std::max(score, bestScore);
                }
            }
            return bestScore;
        } else {
            int bestScore = std::numeric_limits<int>::max();
            for (int i = 0; i < 9; i++) {
                if (board[i] == NONE) {
                    board[i] = HUMAN;
                    int score = minimax(true);
                    board[i] = NONE;
                    bestScore = std::min(score, bestScore);
                }
            }
            return bestScore;
        }
    }
};

// Helper function to draw X and O shapes
void drawX(RenderWindow& window, Vector2f pos, float size, Color color) {
    RectangleShape line1(Vector2f(size, size/6));
    line1.setFillColor(color);
    line1.setPosition(pos);
    line1.setRotation(45);

    RectangleShape line2(Vector2f(size, size/6));
    line2.setFillColor(color);
    line2.setPosition(pos.x, pos.y + size);
    line2.setRotation(-45);

    window.draw(line1);
    window.draw(line2);
}

void drawO(RenderWindow& window, Vector2f center, float radius, Color color) {
    CircleShape circle(radius);
    circle.setOutlineThickness(radius / 6);
    circle.setOutlineColor(color);
    circle.setFillColor(Color::Transparent);
    circle.setPosition(center.x - radius, center.y - radius);
    window.draw(circle);
}

int main() {
    const int windowWidth = 600;
    const int windowHeight = 700;
    RenderWindow window(VideoMode(windowWidth, windowHeight), "TicTacToe - SFML");

    TicTacToeGame game;

    // Board settings
    const int gridSize = 3;
    const float cellSize = 180.f;
    const float boardStartX = (windowWidth - cellSize * gridSize) / 2.f;
    const float boardStartY = 100.f;

    Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        // Use a default SFML font fallback if Arial not found
        std::cerr << "Failed to load font Arial.ttf. Please put Arial.ttf in same folder or change font file path." << std::endl;
        return -1;
    }

    // Buttons for difficulty and restart
    struct Button {
        RectangleShape rect;
        Text text;
        Difficulty diff; // only valid for difficulty buttons
        bool isRestart;
    };

    std::vector<Button> buttons;
    const float btnWidth = 140.f;
    const float btnHeight = 40.f;
    const float btnMargin = 20.f;
    Vector2f btnStart(windowWidth / 2.f - 1.5f * (btnWidth + btnMargin), 20.f);

    std::string diffs[] = {"Easy", "Medium", "Hard"};
    for (int i = 0; i < 3; i++) {
        Button b;
        b.rect = RectangleShape(Vector2f(btnWidth, btnHeight));
        b.rect.setPosition(btnStart.x + i * (btnWidth + btnMargin), btnStart.y);
        b.rect.setFillColor(Color(200, 200, 200));
        b.rect.setOutlineColor(Color::Black);
        b.rect.setOutlineThickness(2.f);
        b.diff = static_cast<Difficulty>(i);
        b.isRestart = false;
        b.text = Text(diffs[i], font, 20);
        b.text.setFillColor(Color::Black);
        FloatRect textBounds = b.text.getLocalBounds();
        b.text.setPosition(
            b.rect.getPosition().x + (btnWidth - textBounds.width) / 2 - textBounds.left,
            b.rect.getPosition().y + (btnHeight - textBounds.height) / 2 - textBounds.top
        );
        buttons.push_back(b);
    }

    // Restart button
    Button restartBtn;
    restartBtn.rect = RectangleShape(Vector2f(btnWidth, btnHeight));
    restartBtn.rect.setPosition(btnStart.x + 3 * (btnWidth + btnMargin), btnStart.y);
    restartBtn.rect.setFillColor(Color(180, 50, 50));
    restartBtn.rect.setOutlineColor(Color::Black);
    restartBtn.rect.setOutlineThickness(2.f);
    restartBtn.isRestart = true;
    restartBtn.text = Text("Restart", font, 20);
    restartBtn.text.setFillColor(Color::White);
    FloatRect textBounds = restartBtn.text.getLocalBounds();
    restartBtn.text.setPosition(
        restartBtn.rect.getPosition().x + (btnWidth - textBounds.width) / 2 - textBounds.left,
        restartBtn.rect.getPosition().y + (btnHeight - textBounds.height) / 2 - textBounds.top
    );
    buttons.push_back(restartBtn);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                // Check buttons
                for (auto& b : buttons) {
                    if (b.rect.getGlobalBounds().contains(mousePos)) {
                        if (b.isRestart) {
                            game.reset();
                        } else {
                            game.setDifficulty(b.diff);
                        }
                    }
                }

                // If game not over and current player is human, register moves
                if (!game.isGameOver() && game.getCurrentPlayer() == HUMAN) {
                    // Check board cells
                    for (int i = 0; i < 9; i++) {
                        int row = i / 3, col = i % 3;
                        FloatRect cellRect(boardStartX + col * cellSize, boardStartY + row * cellSize, cellSize, cellSize);
                        if (cellRect.contains(mousePos)) {
                            game.makeMove(i);
                            break;
                        }
                    }
                }
            }
        }

        // AI move if needed
        if (!game.isGameOver() && game.getCurrentPlayer() == AI) {
            game.aiMove();
        }

        // Drawing
        window.clear(Color(240, 240, 240));

        // Draw buttons
        for (auto& b : buttons) {
            // Highlight selected difficulty
            if (!b.isRestart && b.diff == game.getCurrentPlayer() && game.isGameOver()) {
                b.rect.setFillColor(Color(150, 150, 150));
            } else if (!b.isRestart && b.diff == game.getCurrentPlayer()) {
                b.rect.setFillColor(Color(100, 180, 250));
            } else if (!b.isRestart && b.diff == game.getCurrentPlayer()) {
                b.rect.setFillColor(Color(200, 200, 200));
            }
            if (!b.isRestart && b.diff == game.getCurrentPlayer()) {
                b.rect.setFillColor(Color(100, 180, 250));
            } else if (!b.isRestart) {
                b.rect.setFillColor(Color(200,200,200));
            }

            window.draw(b.rect);
            window.draw(b.text);
        }

        // Draw board grid lines
        Color gridColor(50, 50, 50);
        for(int i=1; i<3; i++) {
            // vertical lines
            Vertex line1[] =
            {
                Vertex(Vector2f(boardStartX + i*cellSize, boardStartY), gridColor),
                Vertex(Vector2f(boardStartX + i*cellSize, boardStartY + gridSize*cellSize), gridColor)
            };
            window.draw(line1, 2, Lines);

            // horizontal lines
            Vertex line2[] =
            {
                Vertex(Vector2f(boardStartX, boardStartY + i*cellSize), gridColor),
                Vertex(Vector2f(boardStartX + gridSize*cellSize, boardStartY + i*cellSize), gridColor)
            };
            window.draw(line2, 2, Lines);
        }

        // Draw X and O in cells
        for(int i=0; i<9; i++) {
            Player p = game.getCell(i);
            if(p == NONE) continue;

            int row = i/3, col = i%3;
            float centerX = boardStartX + col*cellSize + cellSize/2.f;
            float centerY = boardStartY + row*cellSize + cellSize/2.f;

            if(p == HUMAN) {
                drawX(window, Vector2f(centerX - cellSize/3, centerY - cellSize/3), cellSize*2/3, Color::Red);
            } else if (p == AI) {
                drawO(window, Vector2f(centerX, centerY), cellSize/3, Color::Blue);
            }
        }

        // Draw game status text
        Text statusText("", font, 28);
        statusText.setFillColor(Color::Black);

        if (game.isGameOver()) {
            Player win = game.getWinner();
            if (win == HUMAN) {
                statusText.setString("You Win!");
            } else if (win == AI) {
                statusText.setString("You Lose");
            } else {
                statusText.setString("Draw!");
            }
        } else {
            if (game.getCurrentPlayer() == HUMAN)
                statusText.setString("Your turn (X)");
            else
                statusText.setString("AI thinking (O)...");
        }
        FloatRect textBounds = statusText.getLocalBounds();
        statusText.setPosition((windowWidth - textBounds.width) / 2.f, boardStartY + gridSize*cellSize + 20);
        window.draw(statusText);

        window.display();
    }

    return 0;
}

