#pragma once

#include <vector>
#include "battlesnake.hpp"

namespace Battlesnake {
    namespace Minimax {

        struct GameState {
            Board board;
            Snake player;
            Snakes enemies;
        };

        struct SuggestedMove {
            float value;
            Point move;
        };

        using Grid = std::vector<std::vector<BoardElement>>;
        
        class Minimax {
        public:
            Minimax() = default;
            Minimax(int width, int height);
            ~Minimax() = default;

        public:
            // SuggestedMove minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, float alpha, float beta, Point alphaMove, Point betaMove, Points prevEnemyMoves);
            SuggestedMove minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, SuggestedMove alpha, SuggestedMove beta);

            Grid buildWorldMap(const Board& board) const;
            void printWorldMap(const Grid& grid) const;

            Direction direction(const Point& head, const Point& bestMove) const;

            int floodFill(const Point& position, Grid& grid, int open, const bool failsafe = false) const;
            float heuristic(Grid grid, const GameState& state, const Points& playerMoves, const Points& enemyMoves) const;
            
            Points neighbors(const Point& node, const Grid& grid, const bool isTailSafe = true) const;
            bool isSafeSquare(const BoardElement element, const bool isTailSafe = true, const bool failsafe = false) const;

            int distanceTo(const Point& src, const Point& dst) const;

        public:
            int MAX_RECURSION_DEPTH = 8;

        private:
            int m_width;
            int m_height;
            Grid m_grid;
        };
    };
};
