#include <string.h>
#include <iostream>
#include <limits>

#include "json.hpp"
#include "battlesnake.hpp"
#include "router.hpp"
#include "minimax.hpp"
#include "basic.hpp"
#include "logging.hpp"

using namespace std;
using namespace nlohmann;
using namespace Battlesnake;

namespace Battlesnake {
    void from_json(const json& j, Point& p) {
        p.x = j["x"].get<Index>();
        p.y = j["y"].get<Index>();
    }

    void from_json(const json& j, Snake& s) {
        s.body = j["body"].get<Points>();
        s.head = j["head"].get<Point>();
        s.health = j["health"].get<int>();
        s.id = j["id"].get<std::string>();
        s.length = j["length"].get<int>();
        s.name = j["name"].get<std::string>();
    }

    void from_json(const json& j, Board& b) {
        b.height = j["height"].get<int>();
        b.width = j["width"].get<int>();
        b.food = j["food"].get<Points>();
        b.hazards = j["hazards"].get<Points>();
        b.snakes = j["snakes"].get<Snakes>();
    }


    string direction_to_string(const Direction d) {
        switch(d) {
        case Direction::up:
            return "{ \"move\": \"up\" }";
        case Direction::left:
            return "{ \"move\": \"left\" }";
        case Direction::down:
            return "{ \"move\": \"down\" }";
        case Direction::right:
            return "{ \"move\": \"right\" }";
        case Direction::INVALID:
            return "{ \"move\": \"INVALID\" }";
        }
    }
}

void Net::Router::handleRoutes(httplib::Server& server) {
    server.Get("/", [](const auto &, auto &res) {
        string head = "sand-worm";
        string tail = "round-bum";
        string author = "petey9891";
        string color = "#DF6230";
        res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "application/json");
    });

    server.Post("/end", [](const auto &, auto &res){
        res.set_content("ok", "text/plain");
    });

    server.Post("/start", [](const auto &, auto &res){
        res.set_content("ok", "text/plain");
    });

    server.Post("/move", [](auto &req, auto &res){
        try {
            json data = json::parse(req.body);

            Board board = data["board"].get<Board>();
            Snake player = data["you"].get<Snake>();
            Snakes enemies;

            if (board.snakes.size() > 1) {
                std::copy_if(board.snakes.begin(), board.snakes.end(), std::back_inserter(enemies), [player](Snake snake) { return snake.id != player.id; });
            } else {
                enemies.push_back(player);
            }

            Minimax::GameState state = { board, player, enemies };

            Minimax::Minimax paranoid(board.width, board.height);
            if (board.height > 11) {
                if (enemies.size() >= 2) {
                    paranoid.MAX_RECURSION_DEPTH = 1;
                } else if (enemies.size() >= 2) {
                    paranoid.MAX_RECURSION_DEPTH = 2;
                } else {
                    paranoid.MAX_RECURSION_DEPTH = 4;
                }
            } else {
                if (enemies.size() >= 4) {
                    paranoid.MAX_RECURSION_DEPTH = 1;
                } else if (enemies.size() >= 2) {
                    paranoid.MAX_RECURSION_DEPTH = 4;
                }
            }
            // paranoid.MAX_RECURSION_DEPTH = 1;

            LOG(DEBUG, "MAX_RECURSION_DEPTH: ", paranoid.MAX_RECURSION_DEPTH);

            LOG(DEBUG, "MAX_RECURSION_DEPTH: ", paranoid.MAX_RECURSION_DEPTH);

            Minimax::Grid grid = paranoid.buildWorldMap(board);
            
            Minimax::SuggestedMove moveTest = paranoid.minimax(
                grid, 
                state, 
                0, 
                true,
                { std::numeric_limits<float>::lowest(), {} },
                { std::numeric_limits<float>::max(), {} },
                {}
            );

            Direction move = paranoid.direction(player.head, moveTest.move);

            LOG(DEBUG, "[TURN]: ", data["turn"], true);
            LOG(DEBUG, "Moving to: ", moveTest.move);
            LOG(DEBUG, "Moving: ", direction_to_string(move));

            res.set_content(direction_to_string(move), "text/plain");
        } catch (const std::exception& e) {
            cout << "Caught exception \"" << e.what() << "\"\n";
        }
    });
}