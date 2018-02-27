#include "MapBasedGlobalLockImpl.h"

namespace Afina {
namespace Backend {

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Put(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(_m);
    auto k_size = key.size();

    if (key.size() + value.size() > _max_size) {
        return false;
    }
    while (_curr_size + key.size() + value.size() > _max_size) {
         _curr_size -= _list.GetTail()->key.size() + _backend[_list.GetTail()->key]->value.size();
         _backend.erase(_list.GetTail()->key);
         _list.DeleteTail();
    }

    if (_backend.find(key) == _backend.end()) {
        auto node = std::shared_ptr<Entry>(new Entry());
        node->key = key;
        node->value = value;

        _list.AddNode(node);
        _backend[key] = node;
    } else {
        _backend[key]->value = value;
    }
    _curr_size += key.size() + value.size();
    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::PutIfAbsent(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(_m);

    if (_backend.find(key) != _backend.end()) {
        return false;
    }

    return Put(key, value);
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Set(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(_m);

    if (_backend.find(key) == _backend.end()) {
        return false;
    }
    _backend[key]->value = value;

    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Delete(const std::string &key) {
    std::lock_guard<std::mutex> lock(_m);

    if (_backend.find(key) == _backend.end()) {
        return false;
    }

    _list.DeleteNode(_backend[key]);
    _backend.erase(key);

    _curr_size -= key.size() + _backend[key]->value.size();
    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Get(const std::string &key, std::string &value) const {
    std::lock_guard<std::mutex> lock(_m);

    if (_backend.find(key) == _backend.end()) {
        return false;
    }

    value = _backend[key]->value;
    _list.MakeTop(_backend[key]);
    return true;
}

} // namespace Backend
} // namespace Afina
