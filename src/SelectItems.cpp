#include "SelectItems.h"


namespace fastgo {


const int SelectItmes::MAX_INDEX  = 3;

SelectItmes::SelectItmes()
:_selected(1),
_actived(false)
{
    _items.resize(3);
}

void SelectItmes::up() {
    ilog << "_selected " << _selected;
    if (_selected == 1) {
        return;
    }
    --_selected;
}

void SelectItmes::down() {
    ilog << "_selected " << _selected;
    if (_selected == MAX_INDEX) {
        return;
    }
    ++_selected;
}

string SelectItmes::get_prefix(const int index) {
    if (!_actived) {
        return "   ";
    }
    if (index == _selected) {
        return " * ";
    }
    return "   ";
}

void SelectItmes::set_status(const bool active) {
    _actived = active;
    if (!active) {
        _selected = 1;
    }
}

string& SelectItmes::key_word() {
    return _key_word;
}

string SelectItmes::get_item(const int index) {
    if (!_actived) {
        return " ";
    }
    if (index < 1 || index > MAX_INDEX) {
        return "";
    }
    return get_prefix(index) + _items[index-1];
}


void SelectItmes::set_item(const int index, string item) {
    if (index < 1 || index > MAX_INDEX) {
        return;
    }
    _items[index-1] = item;
}


} // namespace fastgo end