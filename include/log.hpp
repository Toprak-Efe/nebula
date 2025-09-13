#ifndef LOG_HPP
#define LOG_HPP

#include <ios>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <ctime>

namespace astronomy {

    enum Levels {
        INFO,
        DEBUG,
        ERROR,
    };

    class Logger {
        public:
            Logger() :
                m_log_file("log.txt", std::ios_base::app)
            {
                if (!m_log_file.is_open()) {
                    std::cerr << "Unable to open log file, exiting." << std::endl;
                    exit(1);
                }
            }
            ~Logger() {
                if (m_log_file.is_open()) {
                    m_log_file.close();
                }
            }
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
                    else if constexpr (E == astronomy::ERROR) {
                        color_code = "\033[31m"; // Red for ERROR
                        msgstream << "[" << getTimestamp() << "]" << " " << color_code << "[ERROR]:" << "\033[0m" << " " << msg << std::endl;
                    }
                    std::cout << msgstream.str();
                    m_log_file << msgstream.str();
                }
        private:
            std::ofstream m_log_file; 
            std::string getTimestamp() {
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                time_t tt = std::chrono::system_clock::to_time_t(now);
                tm tm_now = *localtime(&tt);
                std::stringstream date_s;
                date_s
                    << tm_now.tm_year+1900
                    << "-"
                    << std::format("{:02}", tm_now.tm_mon+1)
                    << "-"
                    << std::format("{:02}", tm_now.tm_mday)
                    << " "
                    << std::format("{:02}", tm_now.tm_hour)
                    << ":"
                    << std::format("{:02}", tm_now.tm_min)
                    << ":"
                    << std::format("{:02}", tm_now.tm_sec);
                return date_s.str();
            }
    };

    inline Logger logger;
};

#endif//LOG_HPP
