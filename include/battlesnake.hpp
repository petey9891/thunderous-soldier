#pragma once

#include <vector>
#include "json.hpp"
#include <string>


namespace Battlesnake {
    enum class BoardElement {
        empty,
        head,
        body,
        tail,
        food,
        hazard,
        filled
    };

    enum class Direction {
        up,
        left,
        right,
        down,
        INVALID
    };

    inline std::ostream & operator<<(std::ostream &os, const Direction& d) {
        switch(d) {
        case Direction::up:
            os << "up";
            break;
        case Direction::left:
            os << "left";
            break;
        case Direction::down:
            os << "down";
            break;
        case Direction::right:
            os << "right";
            break;
        case Direction::INVALID:
            os << "INVALID";
            break;
        }
        return os;
    }

    // Used for positions on board, to make it easier to switch integer type.
    using Index = int;

    struct Point {
        Point() {};
        Point (const Index t_x, const Index t_y) : x(t_x), y(t_y) {}

        Index x = 0;
        Index y = 0;
    };

    inline bool operator==(const Point& lhs, const Point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    };

    inline bool operator!=(const Point& lhs, const Point& rhs) {
        return lhs.x != rhs.x && lhs.y != rhs.y;
    };

    inline std::ostream& operator<<(std::ostream &os, const Point& point) {
        os << "x: " << point.x << " y: " << point.y << "\n";
        return os;
    }

    using Points = std::vector<Point>;

    struct Snake {
        int health;
        int length;
        Point head;
        Points body;
        std::string id;
        std::string name;
    };

    using Snakes = std::vector<Snake>;

    struct Board {
        int height;
        int width;
        Points food;
        Points hazards;
        Snakes snakes;
    };

    inline void from_json(const nlohmann::json& j, Point& p) {
        p.x = j["x"].get<Index>();
        p.y = j["y"].get<Index>();
    }

    inline void from_json(const nlohmann::json& j, Snake& s) {
        s.body = j["body"].get<Points>();
        s.head = j["head"].get<Point>();
        s.health = j["health"].get<int>();
        s.id = j["id"].get<std::string>();
        s.length = j["length"].get<int>();
        s.name = j["name"].get<std::string>();
    }

    inline void from_json(const nlohmann::json& j, Board& b) {
        b.height = j["height"].get<int>();
        b.width = j["width"].get<int>();
        b.food = j["food"].get<Points>();
        b.hazards = j["hazards"].get<Points>();
        b.snakes = j["snakes"].get<Snakes>();
    }


    inline std::string direction_to_string(const Direction d) {
        switch(d) {
            case Direction::up:
                return R"({ "move": "up" })";
            case Direction::left:
                return R"({ "move": "left" })";
            case Direction::down:
                return R"({ "move": "down" })";
            case Direction::right:
                return R"({ "move": "right" })";
            case Direction::INVALID:
                return R"({ "move": "INVALID" })";
        }
    }
};
