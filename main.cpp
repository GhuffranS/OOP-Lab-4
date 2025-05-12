#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;

enum Player { NONE = 0, PLAYER_X, PLAYER_O };

const int SIZE = 3;
const int CELL_SIZE = 200;
const int WINDOW_SIZE = CELL_SIZE * SIZE;
const int LINE_THICKNESS = 4;

struct GameState {
    int board[SIZE][SIZE] = {0};
    Player current = PLAYER_X;
    bool gameOver = false;
    std::string resultText = "";
    int difficulty = 1; // 0 = Easy, 1 = Medium, 2 = Hard
    bool singlePlayer = true;
    bool selectingMode = true;
};

bool isMovesLeft(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == NONE)
                return true;
    return false;
}

int evaluate(int b[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++)
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2] && b[row][0] != NONE)
            return (b[row][0] == PLAYER_O) ? 10 : -10;
    for (int col = 0; col < SIZE; col++)
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col] && b[0][col] != NONE)
            return (b[0][col] == PLAYER_O) ? 10 : -10;
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2] && b[0][0] != NONE)
        return (b[0][0] == PLAYER_O) ? 10 : -10;
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0] && b[0][2] != NONE)
        return (b[0][2] == PLAYER_O) ? 10 : -10;
    return 0;
}

int minimax(int board[SIZE][SIZE], int depth, bool isMax, int alpha, int beta) {
    int score = evaluate(board);
    if (score == 10 || score == -10)
        return score - depth;
    if (!isMovesLeft(board))
        return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (board[i][j] == NONE) {
                    board[i][j] = PLAYER_O;
                    best = std::max(best, minimax(board, depth + 1, false, alpha, beta));
                    board[i][j] = NONE;
                    alpha = std::max(alpha, best);
                    if (beta <= alpha) break;
                }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (board[i][j] == NONE) {
                    board[i][j] = PLAYER_X;
                    best = std::min(best, minimax(board, depth + 1, true, alpha, beta));
                    board[i][j] = NONE;
                    beta = std::min(beta, best);
                    if (beta <= alpha) break;
                }
        return best;
    }
}

std::pair<int, int> findBestMove(GameState& game) {
    int bestVal = -1000;
    std::pair<int, int> bestMove = {-1, -1};
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (game.board[i][j] == NONE) {
                game.board[i][j] = PLAYER_O;
                int moveVal = minimax(game.board, 0, false, -1000, 1000);
                game.board[i][j] = NONE;
                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
    return bestMove;
}

void aiMove(GameState& game) {
    std::pair<int, int> move;
    if (game.difficulty == 0) {
        std::vector<std::pair<int, int>> choices;
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                if (game.board[i][j] == NONE)
                    choices.push_back({i, j});
        move = choices[rand() % choices.size()];
    } else if (game.difficulty == 1 && rand() % 100 < 50) {
        move = findBestMove(game);
    } else {
        move = findBestMove(game);
    }
    game.board[move.first][move.second] = PLAYER_O;
    game.current = PLAYER_X;
}

bool checkWin(GameState& game, Player player) {
    int p = player;
    for (int i = 0; i < SIZE; i++)
        if ((game.board[i][0] == p && game.board[i][1] == p && game.board[i][2] == p) ||
            (game.board[0][i] == p && game.board[1][i] == p && game.board[2][i] == p))
            return true;
    if ((game.board[0][0] == p && game.board[1][1] == p && game.board[2][2] == p) ||
        (game.board[0][2] == p && game.board[1][1] == p && game.board[2][0] == p))
        return true;
    return false;
}

void checkGameOver(GameState& game) {
    if (checkWin(game, PLAYER_X)) {
        game.gameOver = true;
        game.resultText = "Player X Wins!";
    } else if (checkWin(game, PLAYER_O)) {
        game.gameOver = true;
        game.resultText = game.singlePlayer ? "AI Wins!" : "Player O Wins!";
    } else if (!isMovesLeft(game.board)) {
        game.gameOver = true;
        game.resultText = "Draw!";
    }
}

void resetGame(GameState& game) {
    std::fill(&game.board[0][0], &game.board[0][0] + SIZE * SIZE, NONE);
    game.current = PLAYER_X;
    game.gameOver = false;
    game.resultText = "";
}

void drawMenu(RenderWindow& window, Font& font, GameState& game) {
    window.clear(Color::White);

    Text title("Tic Tac Toe", font, 48);
    title.setPosition(100, 40);
    title.setFillColor(Color::Black);
    window.draw(title);

    Text p1("Single Player", font, 36);
    p1.setPosition(100, 150);
    p1.setFillColor(Color::Black);
    window.draw(p1);

    Text p2("Two Player", font, 36);
    p2.setPosition(100, 210);
    p2.setFillColor(Color::Black);
    window.draw(p2);

    Text d0("Easy", font, 30);
    d0.setPosition(100, 280);
    d0.setFillColor(Color::Black);
    window.draw(d0);

    Text d1("Medium", font, 30);
    d1.setPosition(100, 320);
    d1.setFillColor(Color::Black);
    window.draw(d1);

    Text d2("Hard", font, 30);
    d2.setPosition(100, 360);
    d2.setFillColor(Color::Black);
    window.draw(d2);

    window.display();
}

void drawBoard(RenderWindow& window, GameState& game, Font& font) {
    window.clear(Color::White);
    for (int i = 1; i < SIZE; ++i) {
        RectangleShape lineH(Vector2f(WINDOW_SIZE, LINE_THICKNESS));
        lineH.setPosition(0, i * CELL_SIZE);
        lineH.setFillColor(Color::Black);
        window.draw(lineH);

        RectangleShape lineV(Vector2f(LINE_THICKNESS, WINDOW_SIZE));
        lineV.setPosition(i * CELL_SIZE, 0);
        lineV.setFillColor(Color::Black);
        window.draw(lineV);
    }

    Text xText("X", font, 120);
    xText.setFillColor(Color::Red);
    Text oText("O", font, 120);
    oText.setFillColor(Color::Blue);

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (game.board[i][j] == PLAYER_X) {
                xText.setPosition(j * CELL_SIZE + 40, i * CELL_SIZE + 10);
                window.draw(xText);
            } else if (game.board[i][j] == PLAYER_O) {
                oText.setPosition(j * CELL_SIZE + 40, i * CELL_SIZE + 10);
                window.draw(oText);
            }
        }

    if (game.gameOver) {
        RectangleShape overlay(Vector2f(WINDOW_SIZE, 80));
        overlay.setPosition(0, WINDOW_SIZE / 2 - 40);
        overlay.setFillColor(Color(0, 0, 0, 180));
        window.draw(overlay);

        Text text(game.resultText + " - Press R to Restart", font, 28);
        text.setFillColor(Color::White);
        text.setPosition(20, WINDOW_SIZE / 2 - 20);
        window.draw(text);
    }

    window.display();
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic Tac Toe", Style::Close);
    GameState game;

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Font file not found.\n";
        return 1;
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (game.selectingMode) {
                if (event.type == Event::MouseButtonPressed) {
                    int y = event.mouseButton.y;
                    if (y >= 150 && y <= 190) game.singlePlayer = true;
                    else if (y >= 210 && y <= 250) game.singlePlayer = false;
                    else if (y >= 280 && y <= 310) game.difficulty = 0;
                    else if (y >= 320 && y <= 350) game.difficulty = 1;
                    else if (y >= 360 && y <= 390) game.difficulty = 2;
                    if (y >= 150 && y <= 390) game.selectingMode = false;
                }
            } else {
                if (event.type == Event::MouseButtonPressed && !game.gameOver) {
                    int row = event.mouseButton.y / CELL_SIZE;
                    int col = event.mouseButton.x / CELL_SIZE;
                    if (game.board[row][col] == NONE) {
                        game.board[row][col] = game.current;
                        checkGameOver(game);
                        if (!game.gameOver) {
                            if (game.singlePlayer) {
                                game.current = PLAYER_O;
                                aiMove(game);
                                checkGameOver(game);
                            } else {
                                game.current = (game.current == PLAYER_X) ? PLAYER_O : PLAYER_X;
                            }
                        }
                    }
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                    resetGame(game);
                    game.selectingMode = true;
                }
            }
        }
        if (game.selectingMode)
            drawMenu(window, font, game);
        else
            drawBoard(window, game, font);
    }

    return 0;
}
