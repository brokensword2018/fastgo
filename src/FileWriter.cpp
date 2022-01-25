#include "FileWriter.h"




namespace fastgo {

FileWriter::FileWriter(const std::string filename, bool append) {
    if (append) {
        _file.open(filename, std::ios::app);
    } else {
        _file.open(filename);
    }
}

FileWriter::~FileWriter() {
    _file.close();
}

void FileWriter::write(const std::string& text) {
    _file << text;
}

} // namespace fastgo end