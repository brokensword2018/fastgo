#pragma once
#include "common.h"






namespace fastgo {

class PathManager {
public:
    static void load();
    static void load(const char* path, const int level);
    static void load_impl(const char* path, int cur, const int level);
    static void get_path(vector<string>& paths);
};





} // namespace fastgo end