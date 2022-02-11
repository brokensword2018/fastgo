#pragma once 
#include "common.h"



namespace fastgo {

class SearchEngine {
public:
static bool deleteAllPath();
static bool setPathSortableAttributes();
static bool updatePath(const vector<string>& paths);
static vector<string> searchPath(const string& key_words);

};





} // namespace fastgo end