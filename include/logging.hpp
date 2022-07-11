#pragma once

#include <iostream>
#include <string>
#include <format>

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


// void inline LOG(LogLevel level, std::string format, ...) {
//     if (validateLog(level)) {
//         va_list arg;
//         // std::string logMessage = "["+ levelToSring(level) + "] " + format.c_str();
//         format.insert(0, "["+ levelToSring(level) + "] ");
//         va_start(arg, format);

//         vfprintf(stdout, format.c_str(), arg);
//         va_end(arg);
//     }
// }
