#ifndef S21_MULTISET_TPP
#define S21_MULTISET_TPP

#include "s21_multiset.h"

namespace s21 {
template <typename Key>
multiset<Key>::multiset() : RBTree<Key, Key>() {}

template <typename Key>
multiset<Key>::multiset(std::initializer_list<value_type> const& items)
    : RBTree<Key, Key>() {
  for (const auto& item : items) {
    this->insert(item);
  }
}

template <typename Key>
multiset<Key>::multiset(const multiset& ms) : RBTree<Key, Key>(ms) {}

template <typename Key>
multiset<Key>::multiset(multiset&& ms) noexcept
    : RBTree<Key, Key>(std::move(ms)) {}

template <typename Key>
multiset<Key>& multiset<Key>::operator=(const multiset& ms) {
  if (this != &ms) {
    RBTree<Key, Key>::operator=(ms);
  }
  return *this;
}

template <typename Key>
multiset<Key>& multiset<Key>::operator=(multiset&& ms) noexcept {
  if (this != &ms) {
    RBTree<Key, Key>::operator=(std::move(ms));
  }
  return *this;
}

template <typename Key>
void multiset<Key>::clear() {
  RBTree<Key, Key>::clear(RBTree<Key, Key>::root);
  RBTree<Key, Key>::root = nullptr;
  RBTree<Key, Key>::node_count = 0;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::insert(
    const value_type& value) {
  auto result = RBTree<Key, Key>::insertMulti(value, value);
  return iterator(RBTree<Key, Key>::root, result.first);
}

template <typename Key>
void multiset<Key>::erase(iterator pos) {
  if (pos != this->end()) {
    RBTree<Key, Key>::erase(*pos);
  }
}

template <typename Key>
void multiset<Key>::swap(multiset& other) {
  std::swap(RBTree<Key, Key>::root, other.root);
  std::swap(RBTree<Key, Key>::node_count, other.node_count);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::find(const key_type& key) {
  return iterator(RBTree<Key, Key>::root, RBTree<Key, Key>::findNode(key));
}

template <typename Key>
typename multiset<Key>::const_iterator multiset<Key>::find(
    const key_type& key) const {
  return const_iterator(RBTree<Key, Key>::root,
                        RBTree<Key, Key>::findNode(key));
}

template <typename Key>
template <typename... Args>
std::vector<typename multiset<Key>::iterator> multiset<Key>::insert_many(
    Args&&... args) {
  std::vector<iterator> result;
  (result.push_back(this->insert(std::forward<Args>(args))), ...);
  return result;
}
}  // namespace s21

#endif  // S21_MULTISET_TPP
