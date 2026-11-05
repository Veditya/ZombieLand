#ifndef LOGGER
#define LOGGER

#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

class Logger
{
public:
    static bool initialize()
    {
        const std::lock_guard<std::mutex> lock(outputMutex);
        if (!initialized)
        {
            logFile.open("log.txt", std::ios::out | std::ios::trunc);
            initialized = true;
        }
        return logFile.is_open();
    }

    template<typename... Args>
    static void error(const char* file, int line, Args&&... args)
    {
        write(std::cerr, "ERROR", file, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warning(const char* file, int line, Args&&... args)
    {
        write(std::cerr, "WARNING", file, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(const char* file, int line, Args&&... args)
    {
        write(std::cout, "INFO", file, line, std::forward<Args>(args)...);
    }

private:
    inline static std::mutex outputMutex;
    inline static std::ofstream logFile;
    inline static bool initialized = false;

    template<typename... Args>
    static void write(std::ostream& output, const char* level, const char* file,
                      int line, Args&&... args)
    {
        std::ostringstream message;
        (message << ... << std::forward<Args>(args));

        initialize();

        const std::lock_guard<std::mutex> lock(outputMutex);
        const auto formattedMessage = std::string("[") + level + "] " + file + ':' +
                                      std::to_string(line) + " - " + message.str() + '\n';
        output << formattedMessage;
        if (logFile.is_open())
        {
            logFile << formattedMessage;
            logFile.flush();
        }
    }
};

#define LOG_ERROR(...) Logger::error(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::warning(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) Logger::info(__FILE__, __LINE__, __VA_ARGS__)

#endif