#pragma once

#include <iostream>
#include <string>

#include "json.hpp"
#include "battlesnake.hpp"

enum LogLevel {
    INFO,
    DEBUG
};

#define CURRENT_LOG_LEVEL INFO

std::string inline levelToSring(LogLevel level) {
    switch (level) {
        case INFO: 
            return "INFO";
        case DEBUG:
            return "DEBUG";
        default:
            return "INFO"; 
    }
}

bool inline validateLog(LogLevel level) {
    if (level > CURRENT_LOG_LEVEL) {
        return false;
    }
    return true;
}

void inline LOG(LogLevel level, std::string message, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << std::endl;
    }
}

void inline LOG(LogLevel level, std::string message, Battlesnake::Point point, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << point;
    }
}

void inline LOG(LogLevel level, std::string message, Battlesnake::Direction direction, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << direction << std::endl;
    }
}

void inline LOG(LogLevel level, std::string message, nlohmann::json data, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << data << std::endl;
    }
}

void inline LOG(LogLevel level, std::string message, int value, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << value << std::endl;
    }
}

void inline LOG(LogLevel level, std::string message, float value, bool extraSpacer = false) {
    if (validateLog(level)) {
        if (extraSpacer) std::cout << std::endl;
        std::cout << "[" << levelToSring(level) << "] " << message << value << std::endl;
    }
}

class Stopwatch {
    public:
        Stopwatch() = default;
        ~Stopwatch() = default;

    public:
        void start() {
            this->startTime = std::chrono::high_resolution_clock::now();
        }

        void end() {
            this->endTime = std::chrono::high_resolution_clock::now();
        }

        double results(std::string title) {
            std::chrono::duration<double> elapsed = this->endTime - this->startTime; 
            std::cout << title << ": " << elapsed.count() * 1000 << " ms\n";
            return elapsed.count() * 1000.0;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
};