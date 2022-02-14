#include "logger.h"
#include "util.h"





namespace fastgo {

FileWriter* log_file = nullptr;


LogContext::LogContext(const LogLevel level, const std::string& filename, const uint32_t line, const string& func_name) {
    _prefix << log_level_map.at(level) << " ";
    _prefix << util::get_fmt_time() << " ";
    _prefix << "[" << gettid() << "-" << util::get_thread_name() << "] ";
    _prefix << util::get_filename_from_path(filename) << ":" << line << " " << func_name <<  " | ";

    _log.flags(ios::left);
    _log.precision(8);
}

LogContext::~LogContext() {
    log_file->write(_prefix.str() + _log.str() + "\n");
}


void init_log() {
    string log_path = util::get_exe_path() + string("1.log");
    log_file = new FileWriter(log_path);
}

} // namespace fastgo end
