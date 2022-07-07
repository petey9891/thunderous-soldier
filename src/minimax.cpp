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

        float Minimax::minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer) {
            Points moves = this->neighbors(state.player.head, grid);

            std::cout << "Moves size: " << moves.size() << std::endl;
            if (depth == 0) {
                std::cout << "Heuristic:" << std::endl;
                float heuristic = this->heuristic(grid, state, moves);
                return heuristic;
            };

            float value;
            if (maximizingPlayer) {
                value = std::numeric_limits<float>::min();
                std::cout << "Maximizing" << std::endl;
                for (Point move : moves) {
                    std::cout << move << std::endl;
                    value = std::max(value, this->minimax(grid, state, depth - 1, false));
                }
                return value;
            } else {
                value = std::numeric_limits<float>::max();
                std::cout << "Minimixing" << std::endl;
                for (Point move : moves) {
                    std::cout << move << std::endl;
                    value = std::min(value, this->minimax(grid, state, depth - 1, true));
                }
                return value;
            }
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

        float Minimax::heuristic(Grid grid, const GameState& state, Points playerMoves) {
            float score = 0.0f;
            
            // Calculate my score

            if (playerMoves.empty()) {
                return std::numeric_limits<float>::min();
            }

            Grid newGrid = grid;
            const int availableSquares = this->floodFill(state.player.head, newGrid, 0, true);
            printf("availableSquares: %d\n", availableSquares);
            
            const float percentAvailable = (float) availableSquares / (float) (this->m_width * this->m_height);
            printf("percentAvailable: %f\n", percentAvailable);

            if (availableSquares <= state.player.length) {
                return -9999999.0f * (1.0f / percentAvailable);
            }


            if (state.player.head.x == 0 ||
                state.player.head.x == this->m_width - 1 ||
                state.player.head.y == 0 ||
                state.player.head.y == this->m_height - 1
            ) {
                score -= 25000;
            }

            Grid enemyGrid = grid;
            const int enemyAvailableSquares = this->floodFill(state.enemies[0].head, enemyGrid, 0, true);
            printf("enemyAvailableSquares: %d\n", enemyAvailableSquares);
            
            const float enemyPercentAvailable = (float) enemyAvailableSquares / (float) (this->m_width * this->m_height);
            printf("enemyPercentAvailable: %f\n", enemyPercentAvailable);

            if (enemyAvailableSquares <= state.enemies[0].length) {
                score += 9999999;
            }

            if (score < 0.0f) {
                score = score * (1.0f / percentAvailable);
            } else if (score > 0.0f) {
                score = score * percentAvailable;
            }

            printf("score: %f\n", score);

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
