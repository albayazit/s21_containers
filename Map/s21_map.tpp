#ifndef S21_MAP_TPP
#define S21_MAP_TPP

#include "s21_map.h"

namespace s21 {
    template <typename Key, typename T>
    map<Key, T>::map(std::initializer_list<value_type> const &items) : RBTree<key_type, mapped_type>() {
        for (const auto& item : items) {
            this->insert(item.first, item.second);
        }
    }

    template <typename Key, typename T>
    map<Key, T>& map<Key, T>::operator=(map &&m) noexcept {
        if (this != &m) {
            RBTree<key_type, mapped_type>::operator=(std::move(m));
        }
        return *this;
    }

    template <typename Key, typename T>
    T& map<Key, T>::at(const Key& key) {
        return RBTree<key_type, mapped_type>::at(key);
    }

    template <typename Key, typename T>
    const T& map<Key, T>::at(const Key& key) const {
        return RBTree<key_type, mapped_type>::at(key);
    }

    template <typename Key, typename T>
    T& map<Key, T>::operator[](const Key& key) {
        return RBTree<key_type, mapped_type>::getOrInsert(key);
    }

    template <typename Key, typename T>
    std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(const value_type& value) {
        // Вызов метода insert у родительского класса RBTree
        auto [node, inserted] = RBTree<Key, T>::insert(value.first, value.second); // здесь используется прямой вызов метода

        // Проверяем, был ли узел вставлен
        return {iterator(node, this), inserted};  // передаем указатель на узел для создания итератора
    }

    template <typename Key, typename T>
    std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(const Key& key, const T& obj) {
        auto [node, inserted] = RBTree<key_type, mapped_type>::insert(key, obj);
        return {iterator(node, this), inserted};  // Передаем указатель на `map` для итератора
    }

    template <typename Key, typename T>
    std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(const Key& key, const T& obj) {
        auto node = this->findNode(key);
        if (node) {
            node->value = obj;
            return {iterator(node, this), false};
        } else {
            return insert(key, obj);
        }
    }

    template <typename Key, typename T>
    void map<Key, T>::erase(iterator pos) {
        if (pos != end()) {
            Key key_to_erase = (*pos).first;
            RBTree<Key, T>::erase(key_to_erase);  // Удаляем элемент по ключу
        }
    }

    template <typename Key, typename T>
    void map<Key, T>::swap(map& other) {
        std::swap(this->root, other.root);
        std::swap(this->node_count, other.node_count);
    }

    template <typename Key, typename T>
    void map<Key, T>::merge(map& other) {
        // Используем временный итератор, чтобы избежать проблем с присваиванием
        for (auto it = other.begin(); it != other.end(); ) {
            // Сохраняем ключ и значение элемента
            auto key = it->first;
            auto value = it->second;

            // Вставляем элемент в основное дерево
            auto [inserted_it, inserted] = this->insert_or_assign(key, value);

            // Получаем следующий итератор, прежде чем удалять текущий элемент
            ++it;

            // Если элемент был успешно вставлен в основное дерево, удаляем его из `other`
            if (inserted) {
                other.erase(other.find(key));
            }
        }
    }

    template <typename Key, typename T>
    bool map<Key, T>::contains(const Key& key) const {
        return this->findNode(key) != nullptr;
    }

    template <typename Key, typename T>
    typename map<Key, T>::iterator map<Key, T>::find(const key_type& key) {
        auto node = this->findNode(key);  // Ищем узел с заданным ключом
        if (node) {
            return iterator(typename RBTree<Key, T>::iterator(this->root, node), this);
        } else {
            return end();  // Если узел не найден, возвращаем end()
        }
    }

    template <typename Key, typename T>
    template <typename... Args>
    std::vector<std::pair<typename map<Key, T>::iterator, bool>> map<Key, T>::insert_many(Args&&... args) {
        std::vector<std::pair<iterator, bool>> results;

        // Используем fold expression, чтобы обработать каждый аргумент
        (..., results.push_back(this->insert(std::forward<Args>(args))));

        return results;
    }

} // namespace s21

#endif