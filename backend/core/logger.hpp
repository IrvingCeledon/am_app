#pragma once
#include <fstream>
#include <mutex>

class Logger {
public:
    static Logger& instance();

    void debug(const std::string&);
    void info(const std::string&);
    void error(const std::string&);

private:
    std::ofstream file;
    std::mutex mtx;

    Logger(); // privado (singleton)
};
