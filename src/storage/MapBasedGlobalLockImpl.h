#ifndef AFINA_STORAGE_MAP_BASED_GLOBAL_LOCK_IMPL_H
#define AFINA_STORAGE_MAP_BASED_GLOBAL_LOCK_IMPL_H

#include <unordered_map>
#include <mutex>
#include <string>
#include <functional>

#include <afina/Storage.h>
#include "LRUList.h"

namespace Afina {
namespace Backend {

/**
 * # Map based implementation with global lock
 *
 *
 */

using BackendMap = std::unordered_map<std::reference_wrapper<const std::string>,
        Entry&,
        std::hash<std::string>,
        std::equal_to<std::string>>;

class MapBasedGlobalLockImpl : public Afina::Storage {
public:
    MapBasedGlobalLockImpl(size_t max_size = 1024) : _max_size(max_size), _curr_size(0) {}
    ~MapBasedGlobalLockImpl() {}

    // Implements Afina::Storage interface
    bool Put(const std::string &key, const std::string &value) override;

    // Implements Afina::Storage interface
    bool PutIfAbsent(const std::string &key, const std::string &value) override;

    // Make final put in Put and PutIfAbsent methods
    bool SimplePut(const std::string &key, const std::string &value);

    // Implements Afina::Storage interface
    bool Set(const std::string &key, const std::string &value) override;

    // Implements Afina::Storage interface
    bool Delete(const std::string &key) override;

    // Implements Afina::Storage interface
    bool Get(const std::string &key, std::string &value) const override;

private:
    size_t _max_size;
    size_t _curr_size;
    BackendMap _backend;
    mutable LRUList _list;
    mutable std::mutex _m;
};

} // namespace Backend
} // namespace Afina

#endif // AFINA_STORAGE_MAP_BASED_GLOBAL_LOCK_IMPL_H
