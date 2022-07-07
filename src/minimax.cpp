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

        SuggestedMove Minimax::minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, float alpha, float beta, Point alphaMove, Point betaMove, Points prevEnemyMoves) {            
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
                float heuristic = this->heuristic(grid, state, playerMoves, enemyMoves);
                return { heuristic, { -1, -1 } };
            };

            if (maximizingPlayer) {
                for (Point move : moves) {
                    Grid newGrid = grid;
                    GameState newState = state;

                    newGrid[state.player.head.x][state.player.head.y] = BoardElement::body;
                    newGrid[move.x][move.y] = BoardElement::head;

                    newState.player.head = move;
                    newState.player.body.insert(newState.player.body.begin(), move);

                    SuggestedMove newAlpha = this->minimax(newGrid, newState, depth - 1, false, alpha, beta, alphaMove, betaMove, enemyMoves);

                    if (newAlpha.value > alpha) {
                        alpha = newAlpha.value;
                        alphaMove = move;
                    }

                    if (beta <= alpha) {
                        break;
                    }
                }
                return { alpha, alphaMove };
            } else {
                for (Point move : moves) {
                    Grid newGrid = grid;
                    GameState newState = state;

                    newGrid[state.enemies[0].head.x][state.enemies[0].head.y] = BoardElement::body;
                    newGrid[move.x][move.y] = BoardElement::head;

                    newState.enemies[0].head = move;
                    newState.enemies[0].body.insert(newState.enemies[0].body.begin(), move);

                    SuggestedMove newBeta = this->minimax(newGrid, newState, depth - 1, true, alpha, beta, alphaMove, betaMove, {});

                    if (newBeta.value < beta) {
                        beta = newBeta.value;
                        betaMove = move;
                    }

                    if (beta <= alpha) {
                        break;
                    }
                }
                return { beta, betaMove };
            }
        }

        Direction Minimax::direction(const Point& head, const Point& bestMove) const {
            if (head.x == bestMove.x + 1 && head.y == bestMove.y) {
                return Direction::right;
            } else if (head.x == bestMove.x - 1 && head.y == bestMove.y) {
                return Direction::left;
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

        float Minimax::heuristic(Grid grid, const GameState& state, Points playerMoves, Points enemyMoves) {
            float score = 0.0f;
            
            // Calculate my score
            if (playerMoves.empty()) {
                return std::numeric_limits<float>::min();
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
                        case BoardElement::body:
                            printf("[o] ");
                            break;
                        case BoardElement::head:
                            printf("[X] ");
                            break;
                        case BoardElement::hazard:
                            printf("[!] ");
                            break;
                        case BoardElement::food:
                            printf("[#] ");
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

            const int MAX_HEIGHT = this->m_height - 1;

            // Place food
            for (Point food : board.food) {
                grid[food.x][food.y] = BoardElement::food;
            }

            // Place snakes
            for (Snake snake : board.snakes) {
                bool head = true;
                for (Point node : snake.body) {
                    if (head) {
                        grid[node.x][node.y] = BoardElement::head;
                        head = false;
                    } else {
                        grid[node.x][node.y] = BoardElement::body;
                    }
                }
            }
            
            return grid;
        }
    };
};
