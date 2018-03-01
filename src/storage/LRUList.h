#ifndef AFINA_STORAGE_LRU_LIST_H
#define AFINA_STORAGE_LRU_LIST_H

#include <memory>
#include <unordered_map>

namespace Afina {
namespace Backend {

struct Entry
{
    std::string value;
    std::unordered_map<std::string, std::shared_ptr<Entry>>::iterator iter;
    std::weak_ptr<Entry> next;
    std::weak_ptr<Entry> prev;
};

class LRUList {
public:
    LRUList() : _head(nullptr), _tail(nullptr) {};

    void AddNode(std::shared_ptr<Entry>& node);
    void DeleteNode(std::shared_ptr<Entry>& node);
    void DeleteTail();
    void MakeTop(std::shared_ptr<Entry>& node);

    std::shared_ptr<Entry> GetTail() const { return _tail;}

private:
    std::shared_ptr<Entry> _head;
    std::shared_ptr<Entry> _tail;
};

} // namespace Backend
} // namespace Afina

#endif // AFINA_STORAGE_LRU_LIST_H
