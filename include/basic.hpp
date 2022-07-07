
#include "battlesnake.hpp"

namespace Battlesnake {
    namespace Basic {
        class BasicSnake {
        private:
            struct PossibleMoves {
                bool up = true;
                bool down = true;
                bool left = true;
                bool right = true;
            } m_possibleMoves;

        public:
            BasicSnake() = default;

        public:
            Direction basic(const Board& board, const Snake& player); 

        private:
            Direction move() const;

            Point getHead(Snake snake);
            Point getNeck(Snake snake);

            void checkNeck(const Snake& player);
            void checkBoundaries(const Board& board, const Snake& player);

            void logPossibleMoves(const std::string location) const;
        };
    };
};