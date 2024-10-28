#ifndef S21_SET_TPP
#define S21_SET_TPP

#include "s21_set.h"

namespace s21 {
    template <typename Key>
    set<Key>::set() : RBTree<Key>() {}

    template <typename Key>
    set<Key>::set(std::initializer_list<value_type> const &items) : RBTree<Key>() {
        for (const auto& item : items) {
            this->insert(item);
        }
    }

    template <typename Key>
    set<Key>::set(const set &s) : RBTree<Key>(s) {}

    template <typename Key>
    set<Key>::set(set &&s) noexcept : RBTree<Key>(std::move(s)) {}

    template <typename Key>
    set<Key>& set<Key>::operator=(set &&s) noexcept {
        if (this != &s) {
            RBTree<Key>::operator=(std::move(s));
        }
        return *this;
    }

    template <typename Key>
    set<Key>& set<Key>::operator=(const set &s) {
        if (this != &s) {
            RBTree<Key>::operator=(s);
        }
        return *this;
    }

    template <typename Key>
    void set<Key>::clear() {
        RBTree<Key>::clear(RBTree<Key>::root);
        RBTree<Key>::root = nullptr;
        RBTree<Key>::node_count = 0;
    }

    template <typename Key>
    std::pair<typename set<Key>::iterator, bool> set<Key>::insert(const value_type& value) {
        auto result = RBTree<Key>::insert(value);  
        return std::make_pair(iterator(RBTree<Key>::root, result.first), result.second); 
    }

    template <typename Key>
    void set<Key>::erase(iterator pos) {
        if (pos != this->end()) {
            RBTree<Key>::erase(*pos);
        }
    }

    template <typename Key>
    void set<Key>::swap(set& other) {
        std::swap(RBTree<Key>::root, other.root);
        std::swap(RBTree<Key>::node_count, other.node_count);
    }

    template <typename Key>
    void set<Key>::merge(set& other) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            this->insert(*it);
        }
        other.clear();
    }

    template <typename Key>
    typename set<Key>::iterator set<Key>::find(const key_type& key) {
        return iterator(RBTree<Key>::root, RBTree<Key>::findNode(key));
    }

    template <typename Key>
    typename set<Key>::const_iterator set<Key>::find(const key_type& key) const {
        return const_iterator(RBTree<Key>::root, RBTree<Key>::findNode(key));
    }

    template <typename Key>
    template <typename... Args>
    std::vector<std::pair<typename set<Key>::iterator, bool>> set<Key>::insert_many(Args&&... args) {
        std::vector<std::pair<iterator, bool>> result;
        (result.push_back(this->insert(std::forward<Args>(args))), ...);
        return result;
    }
}

#endif  // S21_SET_TPP