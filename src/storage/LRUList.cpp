#include "LRUList.h"

namespace Afina {
namespace Backend {
    void LRUList::AddNode(Entry* node) {
        assert(node);

        node->next = _head;
        node->prev = nullptr;
        if (_head != nullptr) {
            _head->prev = node;
        }
        if (_tail == nullptr) {
            _tail = node;
        }
        _head = node;
    }

    LRUList::~LRUList() {
        auto tmp = _head;
        while (tmp != nullptr) {
            auto prev = tmp;
            tmp = tmp->next;
            delete prev;
        }
    }

    void LRUList::DeleteNode(Entry* node) {
        assert(node);

        if (node == _head) {
            _head = node->next;
        } else {
            node->prev->next = node->next;
        }

        if (node == _tail) {
            _tail = node->prev;
        } else {
            node->next->prev = node->prev;
        }

        delete node;
    }

    void LRUList::MakeTop(Entry* node) {
        assert(node);

        if (node == _head) return;
        node->prev->next = node->next;

        if (node != _tail) {
            node->next->prev = node->prev;
        } else {
            node->prev->next = nullptr;
            _tail = node->prev;
        }
        node->prev = nullptr;
        node->next = _head;
        _head->prev = node;
        _head = node;
    }
    
} // namespace Backend
} // namespace Afina
