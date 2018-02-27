#include "LRUList.h"


namespace Afina {
namespace Backend {

void LRUList::AddNode(std::shared_ptr<Entry>& node) {
    node->next = _head;
    node->prev.reset();
    if (_head.get() != nullptr) {
        _head->prev = node;
    }
    if (_tail.get() == nullptr) {
        _tail = node;
    }
    _head = node;
}

void LRUList::DeleteNode(std::shared_ptr<Entry>& node) {
    if (node == _head) {
        _head = node->next.lock();
    } else {
        node->prev.lock()->next = node->next;
    }

    if (node == _tail) {
        _tail = node->prev.lock();
    } else {
        node->next.lock()->prev = node->prev;
    }
}

void LRUList::DeleteTail() {
    if (_tail == _head) {
        _tail = nullptr;
        _head = nullptr;
        return;
    }
    _tail->prev.lock()->next.reset();
    _tail = _tail->prev.lock();
}

void LRUList::MakeTop(std::shared_ptr<Entry>& node) {
    if (node == _head) return;

    node->prev.lock()->next = node->next;
    if (node != _tail) {
        node->next.lock()->prev = node->prev;
    } else {
        node->prev.lock()->next.reset();
        _tail = node->prev.lock();
    }
    node->prev.reset();
    node->next = _head;
    _head->prev = node;
    _head = node;
}

} // namespace Backend
} // namespace Afina
