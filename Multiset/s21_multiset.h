#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include <initializer_list>
#include <vector>

#include "../Tree/s21_tree.h"

namespace s21 {
template <typename Key>
class multiset : public RBTree<Key, Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename RBTree<Key, Key>::iterator;
  using const_iterator = typename RBTree<Key, Key>::const_iterator;
  using size_type = size_t;

  // Конструкторы и деструктор
  multiset();
  multiset(std::initializer_list<value_type> const& items);
  multiset(const multiset& ms);
  multiset(multiset&& ms) noexcept;
  ~multiset() = default;

  // Перегрузки операторов
  multiset& operator=(const multiset& ms);
  multiset& operator=(multiset&& ms) noexcept;

  // Методы класса
  bool empty() const { return RBTree<Key, Key>::size() == 0; }
  size_type size() const { return RBTree<Key, Key>::size(); }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }
  void clear();
  iterator insert(const value_type& value);
  void erase(iterator pos);
  void swap(multiset& other);
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  size_type count(const Key& key) const { return RBTree<Key, Key>::count(key); }
  // Проверяет, содержится ли элемент с указанным ключом
  bool contains(const Key& key) const {
    return RBTree<Key, Key>::contains(key);
  }
  // Возвращает диапазон элементов с данным ключом
  std::pair<iterator, iterator> equal_range(const Key& key) {
    return RBTree<Key, Key>::equal_range(key);
  }
  // Возвращает первый элемент, не меньший key
  iterator lower_bound(const Key& key) {
    return RBTree<Key, Key>::lower_bound(key);
  }
  // Возвращает первый элемент, больший key
  iterator upper_bound(const Key& key) {
    return RBTree<Key, Key>::upper_bound(key);
  }
  // Множественная вставка
  template <typename... Args>
  std::vector<iterator> insert_many(Args&&... args);
};
}  // namespace s21

#include "s21_multiset.tpp"

#endif  // S21_MULTISET_H
