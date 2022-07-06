// HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
// JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "json.hpp"
#include "http_stuff.h"

#include "router.hpp"

using namespace std;
using namespace nlohmann;

int main(void) {
    httplib::Server server;

    try {
        Net::Router router;
        router.handleRoutes(server);

        server.listen("0.0.0.0", 8080);
        cout << "Server started" << endl;
    } catch (std::runtime_error &e) {
        std::cout << "ERROR: " << e.what() << std::endl
                  << "Another instance of this client running?" << std::endl;
    } catch (const std::exception &e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}
