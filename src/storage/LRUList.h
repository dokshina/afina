#ifndef AFINA_STORAGE_LRU_LIST_H
#define AFINA_STORAGE_LRU_LIST_H

#include <memory>
#include <iostream>
#include <cassert>

namespace Afina {
namespace Backend {

struct Entry
{
    std::string key;
    std::string value;
    Entry* next;
    Entry* prev;
};

class LRUList {
public:
    LRUList() : _head(nullptr), _tail(nullptr) {};
    ~LRUList();

    void AddNode(Entry* node);
    void DeleteNode(Entry* node);

    void MakeTop(Entry* node);

    Entry* GetTail() const { return _tail;}

private:
    Entry* _head;
    Entry* _tail;
};

} // namespace Backend
} // namespace Afina

#endif // AFINA_STORAGE_LRU_LIST_H
