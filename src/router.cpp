#include <string.h>
#include <iostream>
#include <limits>

#include "json.hpp"
#include "battlesnake.hpp"
#include "router.hpp"
#include "minimax.hpp"
#include "basic.hpp"

using namespace std;
using namespace nlohmann;
using namespace Battlesnake;

// Used to enable debug printouts.
const bool debug = false;
const bool print_move = true;


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

            if (debug) {
            std::cout << "*************** MOVE ******************" << std::endl;
            std::cout << data.dump(4) << std::endl;
            }

            // Snake snake = data["you"].get<Snake>();
            Board board = data["board"].get<Board>();
            Snake player = data["you"].get<Snake>();
            Snakes enemies;

            if (board.snakes.size() > 1) {
                enemies.assign(board.snakes.begin() + 1, board.snakes.end());
            } else {
                enemies.push_back(player);
            }

            printf("Number of enemies: %d\n", (int) enemies.size());

            Minimax::GameState state = { board, player, enemies };

            Minimax::Minimax paranoid(board.width, board.height);
            Minimax::Grid grid = paranoid.buildWorldMap(board);
            // std::cout << "*************** START ******************" << std::endl;
            
            // paranoid.printWorldMap(grid);
            // std::cout << "*************** VALUE ******************" << std::endl;
            Minimax::SuggestedMove moveTest = paranoid.minimax(
                grid, 
                state, 
                6, 
                true,
                { std::numeric_limits<float>::lowest(), {} },
                { std::numeric_limits<float>::max(), {} },
                {}
            );

            // std::cout << "*************** END ******************" << std::endl;
            // paranoid.printWorldMap(grid);

            Direction move = paranoid.direction(player.head, moveTest.move);

            if (print_move) {
                std::cout << "*************** TURN " << data["turn"] << " ******************" << std::endl;
                std::cout << direction_to_string(move) << endl;
            }

            res.set_content(direction_to_string(move), "text/plain");
        } catch (const std::exception& e) {
            cout << "Caught exception \"" << e.what() << "\"\n";
        }
    });
}