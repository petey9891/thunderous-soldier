//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "json.hpp"
#include "http_stuff.h"
#include "minimax.h"


using namespace std;
using namespace nlohmann;

int main(void) {
  httplib::Server server;
  
  server.Get("/", [](const auto &, auto &res) {
    string head = "default"; //TODO: Change head
    string tail = "default"; //TODO: Change tail
    string author = "petey9891"; //TODO: Change your battlesnake username
    string color = "#888888";  //TODO: Change a hex color
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "application/json");
  });
  
  server.Post("/end", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  
  server.Post("/start", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  
  server.Post("/move", [](auto &req, auto &res){
    const json data = json::parse(req.body);
    cout << data;
    cout << "\n\n";
    Minimax snake;

    try {
        cout << snake.minimax(data) << "\n\n";
        res.set_content("{\"move\": \"" + snake.minimax(data) + "\"}", "text/plain");
      } catch (const std::exception& e) {
        cout << "Caught exception \"" << e.what() << "\"\n";
      }
  });
  
  server.listen("0.0.0.0", 8080);
  cout << "Server started";
}
