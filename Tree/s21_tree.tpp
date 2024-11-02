#ifndef S21_TREE_TPP
#define S21_TREE_TPP

#include "s21_tree.h"

// Конструкторы
template <typename Key, typename Value>
RBTree<Key, Value>::RBTree() : root(nullptr), node_count(0) {}

template <typename Key, typename Value>
RBTree<Key, Value>::RBTree(const RBTree& other) : root(nullptr), node_count(0) {
    if (other.root) {
        root = copySubtree(other.root, nullptr);
        node_count = other.node_count;
    }
}

template <typename Key, typename Value>
RBTree<Key, Value>::RBTree(RBTree&& other) noexcept : root(other.root), node_count(other.node_count) {
    other.root = nullptr;
    other.node_count = 0;
}

// Деструктор
template <typename Key, typename Value>
RBTree<Key, Value>::~RBTree() { clear(root); }

template <typename Key, typename Value>
void RBTree<Key, Value>::clear(Node* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

// Нахождение узла по ключу
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::findNode(const Key& key) const {
    Node* current = root;
    while (current) {
        if (key < current->key) {
            current = current->left;
        }
        else if (key > current->key) {
            current = current->right;
        } 
        else
            return current;
    }
    return nullptr;
}

// Вставка нового узла
template <typename Key, typename Value>
std::pair<typename RBTree<Key, Value>::Node*, bool> RBTree<Key, Value>::insert(const Key& key, const Value& value) {
    Node* new_node = new Node(key, value);
    Node* parent_node = nullptr;
    Node* find_place_node = root;

    while(find_place_node != nullptr) {
        parent_node = find_place_node;
        if (new_node->key < find_place_node->key) {
            find_place_node = find_place_node->left;
        } else if (new_node->key > find_place_node->key) {
            find_place_node = find_place_node->right;
        } else {
            delete new_node;
            return std::make_pair(find_place_node, false);
        }
    }
    new_node->parent = parent_node;
    if (parent_node == nullptr)
        root = new_node;
    else if (new_node->key < parent_node->key)
        parent_node->left = new_node;
    else
        parent_node->right = new_node;
    
    new_node->color = RED;
    insertFixup(new_node);
    node_count++;
    return std::make_pair(new_node, true);
}

// Вставка нового узла (значения могут повторяться, для multiset)
template <typename Key, typename Value>
std::pair<typename RBTree<Key, Value>::Node*, bool> RBTree<Key, Value>::insertMulti(const Key& key, const Value& value) {
    Node* new_node = new Node(key, value);
    Node* parent_node = nullptr;
    Node* find_place_node = root;

    while(find_place_node != nullptr) {
        parent_node = find_place_node;
        if (new_node->key < find_place_node->key)
            find_place_node = find_place_node->left;
        else
            find_place_node = find_place_node->right;
    }

    new_node->parent = parent_node;

    if (parent_node == nullptr)
        root = new_node;
    else if (new_node->key < parent_node->key)
        parent_node->left = new_node;
    else
        parent_node->right = new_node;
    
    new_node->color = RED;
    insertFixup(new_node);
    node_count++;
    return std::make_pair(new_node, true);
}

// Балансировка дерева при вставке узла
template <typename Key, typename Value>
void RBTree<Key, Value>::insertFixup(Node* node) {
    while (node->parent && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            Node* uncle_node = node->parent->parent->right;
            if (uncle_node && uncle_node->color == RED) {
                node->parent->color = BLACK;
                uncle_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent);
            }
        } else {
            Node* uncle_node = node->parent->parent->left;
            if (uncle_node && uncle_node->color == RED) {
                node->parent->color = BLACK;
                uncle_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// Левый поворот дерев
template <typename Key, typename Value>
void RBTree<Key, Value>::leftRotate(Node* node) {
    Node* new_parent = node->right;
    node->right = new_parent->left;
    if (new_parent->left != nullptr) {
        new_parent->left->parent = node;
    }
    new_parent->parent = node->parent;
    if (node->parent == nullptr) {
        root = new_parent;
    } else if (node == node->parent->left) {
        node->parent->left = new_parent;
    } else {
        node->parent->right = new_parent;
    }
    new_parent->left = node;
    node->parent = new_parent;
}

// Правый поворот дерева
template <typename Key,typename Value>
void RBTree<Key, Value>::rightRotate(Node* node) {
    Node* new_parent = node->left;
    node->left = new_parent->right;
    if (new_parent->right != nullptr) {
        new_parent->right->parent = node;
    }
    new_parent->parent = node->parent;
    if (node->parent == nullptr) {
        root = new_parent;
    } else if (node == node->parent->right) {
        node->parent->right = new_parent;
    } else {
        node->parent->left = new_parent;
    }
    new_parent->right = node;
    node->parent = new_parent;
}

// Поиск минимального узла
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::minimum(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Поиск максимального узла
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::maximum(Node* node) const {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

// Проверка на существование узла
template <typename Key, typename Value>
bool RBTree<Key, Value>::contains(const Key& key) const {
    return findNode(key) != nullptr;
}

// Количество узлов в дереве
template <typename Key, typename Value>
typename RBTree<Key, Value>::size_type RBTree<Key, Value>::size() const {
    return node_count;
}

// Удаление узла по ключу
template <typename Key, typename Value>
void RBTree<Key, Value>::erase(const Key& key) {
    Node* node = findNode(key);
    if (node == nullptr)
        return;
    Node* delete_node = node;
    Node* replace_node = nullptr;
    Color orig_color = delete_node->color;

    if (node->left == nullptr) {
        replace_node = node->right;
        transplant(node, node->right);
    } else if (node->right == nullptr) {
        replace_node = node->left;
        transplant(node, node->left);
    } else { // Есть оба потомка
        delete_node = minimum(node->right);
        orig_color = delete_node->color;
        replace_node = delete_node->right;

        if (delete_node->parent == node) {
            if (replace_node) replace_node->parent = delete_node;
        } else {
            transplant(delete_node, delete_node->right);
            delete_node->right = node;
            delete_node->right->parent = delete_node;
        }

        transplant(node, delete_node);
        delete_node->left = node->left;
        delete_node->left->parent = delete_node;
        delete_node->color = node->color;
    }

    delete node;
    node_count--;
    if (orig_color == BLACK && replace_node)
        deleteFixup(replace_node);
}

// Перестановка узлов местами
template <typename Key, typename Value>
void RBTree<Key, Value>::transplant(Node* first_node, Node* second_node) {
    if (first_node->parent == nullptr) {
        root = second_node;
    } else if (first_node == first_node->parent->left) {
        first_node->parent->left = second_node;
    } else {
        first_node->parent->right = second_node;
    }

    if (second_node != nullptr) {
        second_node->parent = first_node->parent;
    }
}

// Балансировка дерева при удалении узла
template <typename Key, typename Value>
void RBTree<Key, Value>::deleteFixup(Node* node) {
    while (node != root && node->color == BLACK) {
        Node* brother = (node == node->parent->left) ? node->parent->right : node->parent->left;
        if (node == node->parent->left) {
            if (brother->color == RED) {
                brother->color = BLACK;
                node->parent->color = RED;
                leftRotate(node->parent);
                brother = node->parent->right;
            }
            handleBrother(node, brother);
        } else {
            if (brother->color == RED) {
                brother->color = BLACK;
                node->parent->color = RED;
                rightRotate(node->parent);
                brother = node->parent->left;
            }
            handleBrother(node, brother);
        }
    }
    node->color = BLACK;
}

// Проверка "брата" узла при балансировке
template <typename Key, typename Value>
void RBTree<Key, Value>::handleBrother(Node* node, Node* brother) {
    if (brother->left->color == BLACK && brother->right->color == BLACK) {
        brother->color = RED;
        node = node->parent;
    } else {
        if (brother->right->color == BLACK) {
            brother->left->color = BLACK;
            brother->color = RED;
            rightRotate(brother);
            brother = node->parent->right;
        }
        brother->color = node->parent->color;
        node->parent->color = BLACK;
        brother->right->color = BLACK;
        leftRotate(node->parent);
        node = root;
    }
}

// Перегрузка оператора = (копирующее присваивание)
template <typename Key, typename Value>
RBTree<Key, Value>& RBTree<Key, Value>::operator=(const RBTree& other) {
    if (this != &other) {
        clear(root);
        root = nullptr;
        node_count = 0;

        if (other.root) {
            root = copySubtree(other.root, nullptr);
            node_count = other.node_count;
        }
    }
    return *this;
}

// Перегрузка оператора = (перемещающее присваивание)
template <typename Key, typename Value>
RBTree<Key, Value>& RBTree<Key, Value>::operator=(RBTree&& other) noexcept {
    if (this != &other) {
        clear(root);
        root = other.root;
        node_count = other.node_count;

        other.root = nullptr;
        other.node_count = 0;
    }
    return *this;
}

// Копирование узла
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::copySubtree(Node* node, Node* parent) {
    if (node == nullptr)
        return nullptr;
    
    Node* new_node = new Node(node->key);
    new_node->color = node->color;
    new_node->parent = parent;

    new_node->left = copySubtree(node->left, new_node);
    new_node->right = copySubtree(node->right, new_node);
    
    return new_node;
}


// Вспомогательные функции для вывода дерева в консоль
template <typename Key, typename Value>
void RBTree<Key, Value>::print() const {
    if (root == nullptr)
        std::cout << "Tree is empty!" << std::endl;
    else {
        char prefix[500] = "";
        printTree(root, prefix, true);
    }
}

template <typename Key, typename Value>
void RBTree<Key, Value>::printTree(Node* node, char prefix[], bool isLeft) const {
    if (node != nullptr) {
        char newPrefix[1000];
        strcpy(newPrefix, prefix);

        if (node->right) {
            strcat(newPrefix, (isLeft ? "│   " : "    "));
            printTree(node->right, newPrefix, false);
            strcpy(newPrefix, prefix);
        }
        std::cout << prefix << (isLeft ? "└── " : "┌── ") 
                  << node->key << " | " << node->value << (node->color == RED ? " (R)" : " (B)") << std::endl;
        strcpy(newPrefix, prefix);
        strcat(newPrefix, (isLeft ? "    " : "│   "));
        if (node->left) {
            printTree(node->left, newPrefix, true);
        }
    }
}

// Реализация методов для итератора
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::iterator::treeMinimum(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::iterator::treeMaximum(Node* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

// Поиск следующего узла
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::iterator::successor(Node* node) const {
    if (node->right) {
        return treeMinimum(node->right);
    }
    Node* parent = node->parent;
    while (parent && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

// Поиск предыдущего узла
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::iterator::predecessor(Node* node) const {
    if (node->left) {
        return treeMaximum(node->left);
    }
    Node* parent = node->parent;
    while (parent && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

// Реализация константных методов для итератора
template <typename Key, typename Value>
const typename RBTree<Key, Value>::Node* RBTree<Key, Value>::const_iterator::treeMinimum(const Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
const typename RBTree<Key, Value>::Node* RBTree<Key, Value>::const_iterator::treeMaximum(const Node* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

template <typename Key, typename Value>
const typename RBTree<Key, Value>::Node* RBTree<Key, Value>::const_iterator::successor(const Node* node) const {
    if (node->right) {
        return treeMinimum(node->right);
    }
    const Node* parent = node->parent;
    while (parent && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

template <typename Key, typename Value>
const typename RBTree<Key, Value>::Node* RBTree<Key, Value>::const_iterator::predecessor(const Node* node) const {
    if (node->left) {
        return treeMaximum(node->left);
    }
    const Node* parent = node->parent;
    while (parent && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

// Возврат значения по ключу
template <typename Key, typename Value>
Value& RBTree<Key, Value>::at(const Key& key) {
    Node* node = findNode(key);
    if (node) {
        return node->value;
    } else {
        throw std::out_of_range("Key not found in map");
    }
}

template <typename Key, typename Value>
const Value& RBTree<Key, Value>::at(const Key& key) const {
    Node* node = findNode(key);
    if (node) {
        return node->value;
    } else {
        throw std::out_of_range("Key not found in map");
    }
}

// Вставка или изменение существующего ключа
template <typename Key, typename Value>
Value& RBTree<Key, Value>::getOrInsert(const Key& key) {
    Node* node = findNode(key);
    if (node) {
        return node->value;
    }
    auto [new_node, inserted] = insert(key, Value{});
    return new_node->value;
}

#endif