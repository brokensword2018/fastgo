#pragma once
#include <string>
#include <iostream>
#include "common.h"
#include "FileWriter.h"

using namespace std;

namespace fastgo {


enum LogLevel {
    debug = 0,
    info,
    warning,
    error,
};

const static map<LogLevel, string> log_level_map = 
{
    {debug, "D"},
    {info, "I"},
    {warning, "W"},
    {error, "E"},
};

class LogContext {
public:
    LogContext(const LogLevel level, const std::string& filename, const uint32_t line, const string& func_name);
    ~LogContext();
    template<typename T>
    LogContext& operator<<(T&& val) {
        _log << std::forward<T>(val);
        return *this;
    }

private:
    ostringstream _prefix;
    ostringstream _log;
};


#define WriteLog(level) LogContext(level, __FILE__, __LINE__, __FUNCTION__)
#define dlog WriteLog(debug)
#define ilog WriteLog(info)
#define wlog WriteLog(warning)
#define elog WriteLog(error)

} // namespace fastgo end
