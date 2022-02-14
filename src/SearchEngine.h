#pragma once 
#include "common.h"



namespace fastgo {

class SearchEngine {
public:
static bool deleteAllPath();
static bool setPathSortableAttributes();
static bool updatePath(const vector<string>& paths);
static vector<string> searchPath(const string& key_words);

static string last_key_words;
static vector<string> last_path_ret;
};





} // namespace fastgo end