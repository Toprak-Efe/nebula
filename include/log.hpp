#ifndef LOG_HPP
#define LOG_HPP

#include <ios>
#include <sstream>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <ctime>

namespace nebula {

    enum Levels {
        INFO,
        DEBUG,
        ERROR,
    };

    class Logger {
        public:
            Logger();
            ~Logger();
            template <Levels E, typename... Args>
                inline void log(const std::format_string<Args...> fmt, Args&&... args) {
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
                    else if constexpr (E == nebula::ERROR) {
                        color_code = "\033[31m"; // Red for ERROR
                        msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[ERROR]:" << "\033[0m" << " " << msg << std::endl;
                    }
                    std::cout << msgstream.str();
                    m_log_file << msgstream.str();
                }

            template <Levels E, typename... Args>
                inline void logs(const std::string &msg) {
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
                    else if constexpr (E == nebula::ERROR) {
                        color_code = "\033[31m"; // Red for ERROR
                        msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[ERROR]:" << "\033[0m" << " " << msg << std::endl;
                    }
                    std::cout << msgstream.str();
                    m_log_file << msgstream.str();
                }
        private:
            std::ofstream m_log_file; 
            std::string getTimestamp() const;
    };

    inline Logger logger;
};

extern "C" void logger_log_info(const char* format, va_list ap); 
extern "C" void logger_log_error(const char* format, va_list ap);
extern "C" void logger_log_debug(const char* format, va_list ap);

#endif//LOG_HPP
