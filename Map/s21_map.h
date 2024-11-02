#ifndef S21_MAP_H
#define S21_MAP_H
#include "../Tree/s21_tree.h"

namespace s21 {
template <typename Key, typename T>
class map : public RBTree<Key, T> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

  // Итератор
  class MapIterator {
   private:
    typename RBTree<Key, T>::iterator tree_iter;
    map<Key, T>* parent_map;

   public:
    MapIterator(typename RBTree<Key, T>::iterator iter, map<Key, T>* map_ptr)
        : tree_iter(iter), parent_map(map_ptr) {}

    value_type operator*() const {
      if (tree_iter == parent_map->end().tree_iter) {
        throw std::out_of_range("Iterator is out of range or uninitialized.");
      }
      Key key = *tree_iter;
      return {key, parent_map->at(key)};
    }

    const value_type* operator->() const {
      if (tree_iter == parent_map->end().tree_iter) {
        throw std::out_of_range("Iterator is out of range or uninitialized.");
      }
      static std::optional<value_type>
          current_value;  // Значение может отсутствовать
      Key key = *tree_iter;
      current_value.emplace(key, parent_map->at(key));
      return &*current_value;
    }
    MapIterator& operator++() {
      ++tree_iter;
      return *this;
    }
    MapIterator operator++(int) {
      MapIterator temp = *this;
      ++tree_iter;
      return temp;
    }
    bool operator==(const MapIterator& other) const {
      return tree_iter == other.tree_iter;
    }
    bool operator!=(const MapIterator& other) const {
      return tree_iter != other.tree_iter;
    }
  };
  using iterator = MapIterator;
  using const_iterator = const MapIterator;
  iterator begin() { return iterator(RBTree<Key, T>::begin(), this); }
  iterator end() { return iterator(RBTree<Key, T>::end(), this); }

  // Конструкторы и деструктор
  map() : RBTree<Key, T>() {}
  map(std::initializer_list<value_type> const& items);
  map(const map& m) : RBTree<Key, T>(m) {}
  map(map&& m) noexcept : RBTree<Key, T>(std::move(m)) {}
  ~map() = default;

  // Перегрузка оператора
  map& operator=(map&& m) noexcept;

  // Методы класса
  T& at(const Key& key);
  const T& at(const Key& key) const;
  T& operator[](const Key& key);
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  bool empty() const { return this->size() == 0; }
  size_type size() const { return this->node_count; }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }
  void clear() { RBTree<Key, T>::clear(this->root); }
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);
  void erase(iterator pos);
  void swap(map& other);
  void merge(map& other);
  bool contains(const Key& key) const;
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};
}  // namespace s21

#include "s21_map.tpp"

#endif