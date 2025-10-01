#include <log.hpp>

nebula::Logger::Logger() :
    m_log_file("log.txt", std::ios_base::app)
{
    if (!m_log_file.is_open()) {
        std::cerr << "Unable to open log file, exiting." << std::endl;
        exit(1);
    }
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

void logger_log_info(const char* format, va_list ap) {
    std::string message;
    va_list args;
    va_copy(args, ap);
    size_t len = vsnprintf(0, 0, format, args);
    message.resize(len + 1);
    vsnprintf(&message[0], len + 1, format, ap);
    message.resize(len);
    nebula::logger.logs<nebula::INFO>(message);
}

void logger_log_error(const char* format, va_list ap) {
    std::string message;
    va_list args;
    va_copy(args, ap);
    size_t len = vsnprintf(0, 0, format, args);
    message.resize(len + 1);
    vsnprintf(&message[0], len + 1, format, ap);
    message.resize(len);
    nebula::logger.logs<nebula::ERROR>(message);
}

void logger_log_debug(const char* format, va_list ap) {
    std::string message;
    va_list args;
    va_copy(args, ap);
    size_t len = vsnprintf(0, 0, format, args);
    message.resize(len + 1);
    vsnprintf(&message[0], len + 1, format, ap);
    message.resize(len);
    nebula::logger.logs<nebula::DEBUG>(message);
}
