#include "logger.h"
#include "util.h"





namespace fastgo {

FileWriter log_file("1.log");


LogContext::LogContext(const LogLevel level, const std::string& filename, const uint32_t line, const string& func_name) {
    _prefix << log_level_map.at(level) << " ";
    _prefix << get_fmt_time() << " ";
    _prefix << "[" << gettid() << "-" << get_thread_name() << "] ";
    _prefix << get_filename_from_path(filename) << ":" << line << " " << func_name <<  " | ";

    _log.flags(ios::left);
    _log.precision(8);
}

LogContext::~LogContext() {
    log_file.write(_prefix.str() + _log.str() + "\n");
}

} // namespace fastgo end
