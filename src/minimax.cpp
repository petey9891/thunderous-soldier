#include "minimax.hpp"
#include <iostream>         // std::cout
#include <limits>           // std::numeric_limits
#include <algorithm>        // std::max


namespace Battlesnake {
    namespace Minimax {

        Minimax::Minimax(int t_width, int t_height) : 
            m_width(t_width),
            m_height(t_height),
            m_grid(std::vector<std::vector<BoardElement>>(t_width, std::vector<BoardElement>(t_height, BoardElement::empty))) {
            
        }

        SuggestedMove Minimax::minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, SuggestedMove alpha, SuggestedMove beta, Points prevEnemyMoves) {            
            Points moves;
            Points playerMoves = this->neighbors(state.player.head, grid);
            Points enemyMoves;
            if (maximizingPlayer) {
                moves = playerMoves;
                enemyMoves = this->neighbors(state.enemies[0].head, grid);
            } else {
                enemyMoves = prevEnemyMoves;
                moves = prevEnemyMoves;
            }

            if (depth == 0) {
                float heuristic = this->heuristic(grid, state, 0, playerMoves, enemyMoves);
                return { heuristic, { -1, -1 } };
            };

            if (maximizingPlayer) {
                // printf("Maximizing:\n");
                for (Point move : moves) {
                    Grid newGrid = grid;
                    GameState newState = state;

                    newGrid[state.player.head.x][state.player.head.y] = BoardElement::body;
                    newGrid[move.x][move.y] = BoardElement::head;
                    newGrid[state.player.body[state.player.body.size() - 1].x][state.player.body[state.player.body.size() - 1].y] = BoardElement::empty;
                    newGrid[state.player.body[state.player.body.size() - 2].x][state.player.body[state.player.body.size() - 2].y] = BoardElement::tail;

                    newState.player.head = move;
                    newState.player.body.insert(newState.player.body.begin(), move);
                    newState.player.body.pop_back();

                    // std::cout << "Moving to: " << move << std::endl;
                    // this->printWorldMap(newGrid);
                    SuggestedMove newAlpha = this->minimax(newGrid, newState, depth - 1, false, alpha, beta, enemyMoves);

                    // printf("newAlpha.value: %f, alpha.value: %f\n", newAlpha.value, alpha.value);
                    if (newAlpha.value > alpha.value) {
                        alpha = { newAlpha.value, move };
                    }

                    if (beta.value <= alpha.value) {
                        break;
                    }

                    // std::cout << "Current move: " << move << std::endl;
                    // std::cout << "Alpha move: " << alpha.move << std::endl;
                }
                return alpha;
            } else {
                // printf("Minimizing:\n");
                for (Point move : moves) {
                    Grid newGrid = grid;
                    GameState newState = state;

                    newGrid[state.enemies[0].head.x][state.enemies[0].head.y] = BoardElement::body;
                    newGrid[move.x][move.y] = BoardElement::head;
                    newGrid[state.enemies[0].body[state.enemies[0].body.size() - 1].x][state.enemies[0].body[state.enemies[0].body.size() - 1].y] = BoardElement::empty;
                    newGrid[state.enemies[0].body[state.enemies[0].body.size() - 2].x][state.enemies[0].body[state.enemies[0].body.size() - 2].y] = BoardElement::tail;

                    newState.enemies[0].head = move;
                    newState.enemies[0].body.insert(newState.enemies[0].body.begin(), move);
                    newState.enemies[0].body.pop_back();

                    // std::cout << "Moving to: " << move << std::endl;
                    // this->printWorldMap(newGrid);
                    SuggestedMove newBeta = this->minimax(newGrid, newState, depth - 1, true, alpha, beta, {});

                    // printf("newBeta.value: %f, beta.value: %f\n", newBeta.value, beta.value);
                    if (newBeta.value < beta.value) {
                        beta = { newBeta.value, move };
                    }

                    if (beta.value <= alpha.value) {
                        break;
                    }

                    // std::cout << "Current move: " << move << std::endl;
                    // std::cout << "Beta move: " << beta.move << std::endl;
                }
                return beta;
            }
        }

        Direction Minimax::direction(const Point& head, const Point& bestMove) const {
            // printf("******** DIRECTION ********\n");
            // std::cout << "Head: " << head << std::endl;
            // std::cout << "Move: " << bestMove << std::endl;

            if (head.x == bestMove.x + 1 && head.y == bestMove.y) {
                return Direction::left;
            } else if (head.x == bestMove.x - 1 && head.y == bestMove.y) {
                return Direction::right;
            } else if (head.x == bestMove.x && head.y == bestMove.y + 1) {
                return Direction::down;
            } else if (head.x == bestMove.x && head.y == bestMove.y - 1) {
                return Direction::up;
            }

            return Direction::down;
        }

        int Minimax::floodFill(const Point& position, Grid& grid, int open, bool failsafe) const {
            if (this->isSafeSquare(grid[position.x][position.y], failsafe)) {
                grid[position.x][position.y] = BoardElement::filled;
                open++;
                Points neighbors = this->neighbors(position, grid);
                for (Point neighbor : neighbors) {
                    open = this->floodFill(neighbor, grid, open);
                }
            }

            return open;
        }

        float Minimax::heuristic(Grid grid, const GameState& state, int enemyIndex, Points playerMoves, Points enemyMoves) {
            float score = 0.0f;
            
            // Calculate my score
            if (playerMoves.empty()) {
                return std::numeric_limits<float>::lowest();
            }

            Grid newGrid = grid;
            const int availableSquares = this->floodFill(state.player.head, newGrid, 0, true);
            const float percentAvailable = (float) availableSquares / (float) (this->m_width * this->m_height);

            if (availableSquares <= state.player.length) {
                return -9999999.0f * (1.0f / percentAvailable);
            }

            // Calculate enemy score
            if (enemyMoves.empty()) {
                score += std::numeric_limits<float>::max();
            }

            Grid enemyGrid = grid;
            const int enemyAvailableSquares = this->floodFill(state.enemies[0].head, enemyGrid, 0, true);
            const float enemyPercentAvailable = (float) enemyAvailableSquares / (float) (this->m_width * this->m_height);

            if (enemyAvailableSquares <= state.enemies[0].length) {
                score += 9999999;
            }

            // Avoid edge of board
            if (state.player.head.x == 0 ||
                state.player.head.x == this->m_width - 1 ||
                state.player.head.y == 0 ||
                state.player.head.y == this->m_height - 1
            ) {
                score -= 25000;
            }

            if (score < 0.0f) {
                score = score * (1.0f / percentAvailable);
            } else if (score > 0.0f) {
                score = score * percentAvailable;
            }

            return score;
        }

        Points Minimax::neighbors(Point node, Grid grid) const {
            Points moves;

            const Point north (node.x,        node.y + 1);
            const Point south (node.x,        node.y - 1);
            const Point east  (node.x + 1,    node.y);
            const Point west  (node.x - 1,    node.y);

            if (north.y >= 0 && north.y < this->m_height && this->isSafeSquare(grid[north.x][north.y])) {
                moves.push_back(north);
            }
            if (south.y >= 0 && south.y < this->m_height && this->isSafeSquare(grid[south.x][south.y])) {
                moves.push_back(south);
            }
            if (east.x >= 0 && east.x < this->m_width && this->isSafeSquare(grid[east.x][east.y])) {
                moves.push_back(east);
            }
            if (west.x >= 0 && west.x < this->m_width && this->isSafeSquare(grid[west.x][west.y])) {
                moves.push_back(west);
            }

            return moves;
        }

        bool Minimax::isSafeSquare(const BoardElement element, bool failsafe) const {
            if (failsafe) {
                return true;
            }
            return element == BoardElement::empty;
        }

        void Minimax::printWorldMap(const Grid& grid) const {
            const int MAX_HEIGHT = this->m_height - 1;
            for (int j = 0; j < this->m_width; j++) {
                for (int i = 0; i < this->m_height; i++) {
                    BoardElement element = grid[i][MAX_HEIGHT - j];
                    switch (element) {
                        case BoardElement::empty:
                            printf("[ ] ");
                            break;
                        case BoardElement::head:
                            printf("[X] ");
                            break;
                        case BoardElement::body:
                            printf("[o] ");
                            break;
                        case BoardElement::tail:
                            printf("[+] ");
                            break;
                        case BoardElement::food:
                            printf("[#] ");
                            break;
                        case BoardElement::hazard:
                            printf("[!] ");
                            break;
                        case BoardElement::filled:
                            printf("[*] ");
                            break;
                    }
                }
                printf("\n");
            }
            printf("\n");
        }

        Grid Minimax::buildWorldMap(const Board& board) {
            Grid grid(this->m_width, std::vector<BoardElement>(this->m_height, BoardElement::empty));

            // Place food
            for (Point food : board.food) {
                grid[food.x][food.y] = BoardElement::food;
            }

            // Place snakes
            for (Snake snake : board.snakes) {
                bool head = true;
                for (int i = 0; i < snake.body.size(); i++) {
                    Point p = snake.body[i];
                    if (i == 0) {
                        grid[p.x][p.y] = BoardElement::head;
                    } else if (i == snake.body.size() - 1) {
                        grid[p.x][p.y] = BoardElement::tail;
                    } else {
                        grid[p.x][p.y] = BoardElement::body;
                    }
                }
            }
            
            return grid;
        }
    };
};
