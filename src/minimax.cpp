#include <iostream>         // std::cout
#include <limits>           // std::numeric_limits
#include <algorithm>        // std::max

#include "minimax.hpp"
#include "logging.hpp"

namespace Battlesnake {
    namespace Minimax {

        Minimax::Minimax(int t_width, int t_height) : 
            m_width(t_width),
            m_height(t_height),
            m_grid(std::vector<std::vector<BoardElement>>(t_width, std::vector<BoardElement>(t_height, BoardElement::empty))) {
            
        }

        SuggestedMove Minimax::minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, SuggestedMove alpha, SuggestedMove beta, Points prevEnemyMoves) {            
            LOG(DEBUG, maximizingPlayer ? "[Maximizing]" : "[Minimizing]", true);
            LOG(DEBUG, "Depth: ", depth);

            Points moves;
            Points playerMoves = this->neighbors(state.player.head, grid);
            
            Points enemyMoves;
            if (maximizingPlayer) {
                moves = playerMoves;
                enemyMoves = this->neighbors(state.enemies[0].head, grid);
            } else {
                enemyMoves = this->neighbors(state.enemies[0].head, grid);
                moves = this->neighbors(state.enemies[0].head, grid);
            }

            if (depth == this->MAX_RECURSION_DEPTH  || moves.empty() || state.player.health <= 0 || state.enemies[0].health <= 0) {
                LOG(DEBUG, "Applying heuristic... returning up the tree");
                float heuristic = this->heuristic(grid, state, playerMoves, enemyMoves);
                return { heuristic, { -1, -1 } };
            };

            // #if (CURRENT_LOG_LEVEL == DEBUG)
            //     LOG(DEBUG, "Number of nodes: ", (int) moves.size());
            //     printf("[DEBUG] Nodes: ");
            //     if (maximizingPlayer) {
            //         for (Point move: moves) {
            //             std::cout << this->direction(state.player.head, move) << ' ';
            //         }
            //         printf("\n");
            //         LOG(DEBUG, "Current position: ", state.player.head);
            //     } else {
            //         for (Point move: moves) {
            //             std::cout << this->direction(state.enemies[0].head, move) << ' ';
            //         }
            //         printf("\n");
            //         LOG(DEBUG, "Current position: ", state.enemies[0].head);
            //     }

            // #endif

            if (maximizingPlayer) {
                for (Point move : moves) {
                    LOG(DEBUG, "Current move: ", this->direction(state.player.head, move));

                    Grid newGrid = grid;
                    GameState newState = state;

                    bool eating = false;
                    if (newGrid[move.x][move.y] == BoardElement::food) {
                        eating = true;
                        newState.player.health = 100;
                        newState.player.length += 1;
                    } else {
                        newState.player.health -= 1;
                    }

                    const int length = newState.player.body.size() - 1;
                    const bool growing = length < 2;
                    if (growing) {
                        newGrid[newState.player.body[length].x][newState.player.body[length].y] = BoardElement::tail;
                    } else {
                        if (!eating) {
                            newGrid[newState.player.body[length - 1].x][newState.player.body[length - 1].y] = BoardElement::tail;
                            newGrid[newState.player.body[length].x][newState.player.body[length].y] = BoardElement::empty;
                            newState.player.body.pop_back();
                        }
                    }

                    // Update snake's head
                    if (length > 0) {
                        newGrid[newState.player.head.x][newState.player.head.y] = BoardElement::body;
                    }
                    newGrid[move.x][move.y] = BoardElement::head;
                    newState.player.head = move;
                    newState.player.body.insert(newState.player.body.begin(), move);

                    SuggestedMove newAlpha = this->minimax(newGrid, newState, depth + 1, false, alpha, beta, enemyMoves);

                    LOG(DEBUG, "Current alpha value: ", alpha.value);
                    LOG(DEBUG, "Current alpha move: ", alpha.move);
                    LOG(DEBUG, "Current newAlpha value: ", newAlpha.value);
                    LOG(DEBUG, "Current newAlpha move: ", move);
                    if (newAlpha.value > alpha.value) {
                        LOG(DEBUG, "Setting alpha value: ", newAlpha.value);
                        LOG(DEBUG, "Setting alpha move: ", move);
                        alpha = { newAlpha.value, move };
                    }

                    if (beta.value <= alpha.value) {
                        break;
                    }
                }
                return alpha;
            } else {
                for (Point move : moves) {
                    LOG(DEBUG, "Current move: ", this->direction(state.enemies[0].head, move));
                    Grid newGrid = grid;
                    GameState newState = state;

                    bool eating = false;
                    if (newGrid[move.x][move.y] == BoardElement::food) {
                        eating = true;
                        newState.enemies[0].health = 100;
                        newState.enemies[0].length += 1;
                    } else {
                        newState.enemies[0].health -= 1;
                    }

                    const int length = newState.enemies[0].body.size() - 1;
                    const bool growing = length < 2;
                    if (growing) {
                        newGrid[newState.enemies[0].body[length].x][newState.enemies[0].body[length].y] = BoardElement::tail;
                    } else {
                        if (!eating) {
                            newGrid[newState.enemies[0].body[length - 1].x][newState.enemies[0].body[length - 1].y] = BoardElement::tail;
                            newGrid[newState.enemies[0].body[length].x][newState.enemies[0].body[length].y] = BoardElement::empty;
                            newState.enemies[0].body.pop_back();    
                        }
                    }

                    // Update snake's head
                    if (length > 0) {
                        newGrid[newState.enemies[0].head.x][newState.enemies[0].head.y] = BoardElement::body;
                    }
                    newGrid[move.x][move.y] = BoardElement::head;
                    newState.enemies[0].head = move;
                    newState.enemies[0].body.insert(newState.enemies[0].body.begin(), move);

                    SuggestedMove newBeta = this->minimax(newGrid, newState, depth + 1, true, alpha, beta, {});

                    if (newBeta.value < beta.value) {
                        beta = { newBeta.value, move };
                    }

                    if (beta.value <= alpha.value) {
                        break;
                    }
                }
                return beta;
            }
        }

        Direction Minimax::direction(const Point& head, const Point& bestMove) const {
            if (head.x == bestMove.x + 1 && head.y == bestMove.y) {
                return Direction::left;
            } else if (head.x == bestMove.x - 1 && head.y == bestMove.y) {
                return Direction::right;
            } else if (head.x == bestMove.x && head.y == bestMove.y + 1) {
                return Direction::down;
            } else if (head.x == bestMove.x && head.y == bestMove.y - 1) {
                return Direction::up;
            }
            
            return Direction::INVALID;
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
                return std::numeric_limits<float>::lowest();
            }

            if (state.player.health <= 0) {
                return std::numeric_limits<float>::lowest();
            }

            // Head to head collisions
            if (state.player.head.x == state.enemies[0].head.x && 
                    state.player.head.y == state.enemies[0].head.y) {
                // If my health is greater
                if (state.player.length > state.enemies[0].length) {
                    return std::numeric_limits<float>::max();
                } else {
                    return std::numeric_limits<float>::lowest();
                }
            }


            Grid newGrid = grid;
            const int availableSquares = this->floodFill(state.player.head, newGrid, 0, true);
            const float percentAvailable = (float) availableSquares / (float) (this->m_width * this->m_height);

            if (availableSquares <= state.player.length) {
                // if (percentAvailable == 0) {
                //     // avoid divide by zero
                //     return -9999999.0f;
                // }
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
                score += 9999999.0f;
            }

            // Fooooooood
            int foodWeight = 0;
            if (state.board.food.size() <= 8) {
                foodWeight = 200 - (2 * state.player.health);
            } else {
                if (state.player.health <= 40 || state.player.body.size() < 4) {
                    foodWeight = 100 - state.player.health;
                }
            }

            if (foodWeight > 0) {
                for (int i = 0; i < state.board.food.size(); i++) {
                    int distance = this->distanceTo(state.player.head, state.board.food[i]);
                    score = score - (distance * foodWeight) - i;
                }
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
            return element == BoardElement::empty || element == BoardElement::food || element == BoardElement::tail;
        }

        void Minimax::printWorldMap(const Grid& grid) const {
            const int MAX_HEIGHT = this->m_height - 1;
            for (int j = 0; j < this->m_height; j++) {
                for (int i = 0; i < this->m_width; i++) {
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

            LOG(DEBUG, "Populated map with food");

            // Place hazards
            for (Point hazard : board.hazards) {
                grid[hazard.x][hazard.y] = BoardElement::hazard;
            }

            LOG(DEBUG, "Populated map with hazards");

            // Place snakes
            for (Snake snake : board.snakes) {
                bool head = true;
                for (int i = 0; i < snake.body.size(); i++) {
                    Point p = snake.body[i];
                    if (i == 0 || snake.head == p) {
                        grid[p.x][p.y] = BoardElement::head;
                    } else if (i == snake.body.size() - 1) {
                        grid[p.x][p.y] = BoardElement::tail;
                    } else {
                        grid[p.x][p.y] = BoardElement::body;
                    }
                }
            }

            LOG(DEBUG, "Populated map with snakes");
            
            return grid;
        }

        int Minimax::distanceTo(const Point& src, const Point& dst) const {
            int dx = std::abs(src.x - dst.x);
            int dy = std::abs(src.y - dst.y);
            return dx + dy;
        }
    };
};
