#include "MapBasedGlobalLockImpl.h"


namespace Afina {
namespace Backend {
// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::PutNode(const std::string &key, const std::string &value) {
    auto it = _backend.find(key);

    if (it != _backend.end()) {
        auto new_size = _curr_size - it->second.value.size() + value.size();

        if (new_size <= _max_size) {
            it->second.value = value;
            _list.MakeTop(&it->second);

            _curr_size = new_size;
            return true;
        }
        _curr_size -= key.size() + it->second.value.size();
        _list.DeleteNode(&it->second);
        _backend.erase(it);
    }

    while (_curr_size + key.size() + value.size() > _max_size) {
        auto tail = _list.GetTail();
        _curr_size -= tail->key.size() + tail->value.size();
        _backend.erase(tail->key);
        _list.DeleteNode(tail);
    }

    auto node = new Entry();
    node->key = key;
    node->value = value;

    _list.AddNode(node);
    _backend.insert(std::make_pair(std::cref(node->key), std::ref(*node)));

    _curr_size += key.size() + value.size();
    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Put(const std::string &key, const std::string &value) {
    if (key.size() + value.size() > _max_size) return false;
    std::lock_guard<std::mutex> lock(_m);

    return PutNode(key, value);
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::PutIfAbsent(const std::string &key, const std::string &value) {
    if (key.size() + value.size() > _max_size) return false;
    std::lock_guard<std::mutex> lock(_m);

    if (_backend.find(key) != _backend.end()) return false;
    return PutNode(key, value);
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Set(const std::string &key, const std::string &value) {
    if (key.size() + value.size() > _max_size) return false;
    std::lock_guard<std::mutex> lock(_m);

    auto it = _backend.find(key);
    if (it == _backend.end()) {
        return false;
    }

    _curr_size = _curr_size - it->second.value.size() + value.size();
    it->second.value = value;
    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Delete(const std::string &key) {
    std::lock_guard<std::mutex> lock(_m);

    auto it = _backend.find(key);
    if (it == _backend.end()) return false;

    _curr_size -= key.size() + it->second.value.size();
    _list.DeleteNode(&it->second);
    _backend.erase(it);
    return true;
}

// See MapBasedGlobalLockImpl.h
bool MapBasedGlobalLockImpl::Get(const std::string &key, std::string &value) const {
    std::lock_guard<std::mutex> lock(_m);

    auto it = _backend.find(key);
    if (it == _backend.end()) return false;

    value = it->second.value;
    _list.MakeTop(&it->second);
    return true;
}

} // namespace Backend
} // namespace Afina
