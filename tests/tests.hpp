#include "json.hpp"
#include "battlesnake.hpp"

#define EVAL_LOG() printf(" line: %d", __LINE__);

namespace Battlesnake {
    void from_json(const nlohmann::json& j, Battlesnake::Point& p) {
        p.x = j["x"].get<Battlesnake::Index>();
        p.y = j["y"].get<Battlesnake::Index>();
    }

    void from_json(const nlohmann::json& j, Battlesnake::Snake& s) {
        s.body = j["body"].get<Battlesnake::Points>();
        s.head = j["head"].get<Battlesnake::Point>();
        s.health = j["health"].get<int>();
        s.id = j["id"].get<std::string>();
        s.length = j["length"].get<int>();
        s.name = j["name"].get<std::string>();
    }

    void from_json(const nlohmann::json& j, Battlesnake::Board& b) {
        b.height = j["height"].get<int>();
        b.width = j["width"].get<int>();
        b.food = j["food"].get<Battlesnake::Points>();
        b.hazards = j["hazards"].get<Battlesnake::Points>();
        b.snakes = j["snakes"].get<Snakes>();
    }
}