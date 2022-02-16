#include "PathManager.h"
#include "util.h"
#include "FileWriter.h"
#include "SearchEngine.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>

namespace fastgo {



void PathManager::load() {
    SearchEngine::deleteAllPath();
    char* exe_path = util::get_exe_path();
    string file_path = string(exe_path) + "path.txt";
    ifstream file;
    file.open(file_path, ios::in);
    if (!file.is_open()) {
        throw runtime_error("open path.txt fail");
    }
    string line;
    while (getline(file, line))
    {
        std::istringstream iss(line);
        string path;
        int level;
        iss >> path >> level;
        load(path.c_str(), level);
    }
    SearchEngine::setPathSortableAttributes();
}


static vector<string> paths;
void PathManager::load(const char* path, const int level) {
    if (!util::is_dir(path)) {
        throw runtime_error(string(path) + "is not a directory");
    }
    if (level < 0) {
        throw runtime_error("level should bigger than zero");
    }
    char abs_path[PATH_MAX];
    if (realpath(path, abs_path)) {
        ilog << "abs_path is " << abs_path;
    } else {
        throw runtime_error("get real path fail");
    }
    load_impl(abs_path, 0, level);
    cout << "load success: " << abs_path  << endl;
    SearchEngine::updatePath(paths);
}

void PathManager::load_impl(const char* path, int cur, const int level) {
    paths.push_back(path);
    if (cur == level) {
        return;
    }
    DIR* dir = opendir(path);
    if (!dir) {
        throw runtime_error("open dir " + string(path) + " fail, " + string(strerror(errno)));
    }
    struct dirent* pdir = nullptr;
    while (nullptr != (pdir = readdir(dir))) {
        const char* name = pdir->d_name;
        const unsigned char type = pdir->d_type;
        if (type != DT_DIR || !strcmp(name, ".") || !strcmp(name, "..") || name[0] == '.') {
            continue;
        }
        string next_level_path = string(path) + "/" + string(name);
        load_impl(next_level_path.c_str(), cur + 1, level);
    }
    
}

void PathManager::get_path(vector<string>& paths) {
    ifstream file;
    file.open("./path.txt", ios::in);
    if (!file.is_open()) {
        throw runtime_error("open path.txt fail");
    }
    string line;
    while (getline(file, line))
    {
        paths.push_back(std::move(line));
    }
}

} // namespace fastgo end