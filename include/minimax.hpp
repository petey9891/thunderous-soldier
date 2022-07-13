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
            SuggestedMove minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer, SuggestedMove alpha, SuggestedMove beta, Points prevEnemyMoves);

            Grid buildWorldMap(const Board& board);
            void printWorldMap(const Grid& grid) const;

            Direction direction(const Point& head, const Point& bestMove) const;

            int floodFill(const Point& position, Grid& grid, int open, bool failsafe = false) const;
            float heuristic(Grid grid, const GameState& state, Points playerMoves, Points enemyMoves);
            
            Points neighbors(Point node, Grid grid, bool isTailSafe = true) const;
            bool isSafeSquare(const BoardElement element, bool isTailSafe = true) const;

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
