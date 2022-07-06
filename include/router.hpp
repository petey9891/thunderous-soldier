#pragma once

#include "http_stuff.h"

namespace Net {
    class Router {
        public:
        void handleRoutes(httplib::Server& server);
    };
}