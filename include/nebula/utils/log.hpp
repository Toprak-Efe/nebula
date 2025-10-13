#pragma once

#include <sstream>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <format>

namespace nebula {

enum class Levels {
    INFO,
    DEBUG,
    ERROR,
};

class Logger {
public:
    static Logger &get();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    template <Levels E, typename... Args>
    void log(const std::format_string<Args...> fmt, Args&&... args) {
        std::string msg = std::vformat(fmt.get(), std::make_format_args(args ...));
        std::string color_code;
        std::stringstream msgstream;
        if constexpr (E == Levels::INFO) {
            color_code = "\033[32m"; // Green for INFO
            msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[INFO]:" << "\033[0m" << " " << msg << std::endl;
        }
        else if constexpr (E == Levels::DEBUG) {
            color_code = "\033[34m"; // Blue for DEBUG
            msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[DEBUG]:" << "\033[0m" << " " << msg << std::endl;
        }
        else if constexpr (E == Levels::ERROR) {
            color_code = "\033[31m"; // Red for ERROR
            msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[ERROR]:" << "\033[0m" << " " << msg << std::endl;
        }
        std::cout << msgstream.str();
        m_log_file << msgstream.str();
        m_log_file.flush();
    }

private:
    Logger();
    std::ofstream m_log_file; 
    std::string getTimestamp() const;
    };
};

