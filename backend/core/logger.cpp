#include "logger.hpp"
#include <iostream>
#include <ctime>

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

Logger::Logger() {
    file.open("data/backend.log");
}

void Logger::debug(const std::string& msg) {
    std::lock_guard lock(mtx);
    file << "[DEBUG] " << msg << "\n";
}
