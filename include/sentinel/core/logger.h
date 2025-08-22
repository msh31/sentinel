//
// Created by Marco on 22/08/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

#define RESET     "\033[0m"
#define ERROR     "\033[31m"
#define WARNING   "\033[33m"
#define INFO      "\033[36m"
#define SUCCESS   "\033[32m" // (or DEBUG)
#define FATAL     "\033[35m"

class logger
{
    public:
        logger();
        ~logger();

        bool consoleLoggingEnabled = true;
        bool fileLoggingEnabled = true;

        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);
        void success(const std::string& message);
        void debug(const std::string& message);
        void fatal(const std::string& message);

    private:
        void log(const std::string& level, const std::string& message);

        std::ofstream logFile;

        std::string logFilePath = "sentinel.log";
        static std::string getColorForLevel(const std::string& level);
};

#endif //LOGGER_H
