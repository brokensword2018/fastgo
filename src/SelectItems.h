#pragma once
#include "common.h"




namespace fastgo {

class SelectItmes {
public:
    SelectItmes();
    ~SelectItmes() = default;
    void up();
    void down();
    void set_status(const bool active);
    string& key_word();
    string get_item(const int index);
    void set_item(const int index, string item);

private:
    string get_prefix(const int index);


    const static int MAX_INDEX;
    int _selected;
    bool _actived;
    string _key_word;
    vector<string> _items;
};





} // namespace fastgo end