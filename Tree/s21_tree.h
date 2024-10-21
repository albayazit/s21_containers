#ifndef S21_TREE_H
#define S21_TREE_H

#include <iostream>
#include <utility> // std::pair

template <typename Key>
class RBTree {
    private:
        enum Color { RED, BLACK };
        struct Node {
            Key key;
            Node* left;
            Node* right;
            Node* parent;
            Color color;
            Node(Key k, Color c = RED) : key(k), left (nullptr), right(nullptr), parent(nullptr), color(c) {}
        };
        Node* root;
        size_t node_count;

        void leftRotate(Node* node);
        void rightRotate(Node* node);
        Node* findNode(const Key& key) const;
        void insertFixup(Node* node);
        Node* minimum(Node* node) const;
        Node* maximum(Node* node) const;
        void transplant(Node* first_node, Node* second_node);
        void deleteFixup(Node* node);
        Node* copySubtree(Node* node, Node* parent);
        void handleBrother(Node* node, Node* brother);
    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = size_t;

        RBTree();
        RBTree(const RBTree& other);
        RBTree(RBTree&& other) noexcept;
        RBTree& operator=(const RBTree& other);
        RBTree& operator=(RBTree&& other) noexcept;
        ~RBTree();
        void clear(Node* node);

        std::pair<Node*, bool> insert(const Key& key);
        void erase(const Key& key);
        bool contains(const Key& key) const;
        void print() const;
        void printTree(Node* node, char prefix[], bool isLeft) const;
        size_type size() const;
};

#include "s21_tree.tpp"

#endif