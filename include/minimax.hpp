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

        using Grid = std::vector<std::vector<BoardElement>>;
        
        class Minimax {
        public:
            Minimax() = default;
            Minimax(int width, int height);
            ~Minimax() = default;

        public:
            float minimax(Grid& grid, const GameState& state, int depth, bool maximizingPlayer);

            Grid buildWorldMap(const Board& board);
            void printWorldMap(const Grid& grid) const;

        private:
            int floodFill(const Point& position, Grid& grid, int open, bool failsafe = false) const;
            float heuristic(Grid grid, const GameState& state, Points moves);
            
            Points neighbors(Point node, Grid grid) const;
            bool isSafeSquare(const BoardElement element, bool failsafe = false) const;

        private:
            int m_width;
            int m_height;
            Grid m_grid;
        };
    };
};
