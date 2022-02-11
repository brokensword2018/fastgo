#pragma once
#include <string>
#include <fstream>

namespace fastgo {

class FileWriter {
public:
    FileWriter(const std::string filename, bool append = false);
    ~FileWriter();
    void write(const std::string& text);

private:
    std::ofstream _file;
};



} // namespace fastgo end