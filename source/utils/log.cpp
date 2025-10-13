#include <chrono>
#include "../../include/nebula/utils/log.hpp"

nebula::Logger::Logger() :
    m_log_file("log.txt", std::ios_base::app)
{
    if (!m_log_file.is_open()) {
        std::cerr << "Unable to open log file, exiting." << std::endl;
        exit(1);
    }
} 

nebula::Logger &nebula::Logger::get() {
    static nebula::Logger logger;
    return logger;
}

nebula::Logger::~Logger() {
    if (m_log_file.is_open()) {
        m_log_file.close();
    }
}

std::string nebula::Logger::getTimestamp() const {
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

