#include "s21_tree.h"

template <typename Key>
RBTree<Key>::RBTree() : root(nullptr), node_count(0) {}

template <typename Key>
RBTree<Key>::RBTree(const RBTree& other) : root(nullptr), node_count(0) {
    if (other.root) {
        root = copySubtree(other.root, nullptr);
        node_count = other.node_count;
    }
}

template <typename Key>
RBTree<Key>::RBTree(RBTree&& other) noexcept : root(other.root), node_count(other.node_count) {
    other.root = nullptr;
    other.node_count = 0;
}

template <typename Key>
RBTree<Key>::~RBTree() { clear(root); }

template <typename Key>
void RBTree<Key>::clear(Node* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename Key>
typename RBTree<Key>::Node* RBTree<Key>::findNode(const Key& key) const {
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

template <typename Key>
std::pair<typename RBTree<Key>::Node*, bool> RBTree<Key>::insert(const Key& key) {
    Node* new_node = new Node(key);
    Node* parent_node = nullptr;
    Node* find_place_node = root;

    while(find_place_node != nullptr) {
        parent_node = find_place_node;
        if (new_node->key < find_place_node->key)
            find_place_node = find_place_node->left;
        else if (new_node->key > find_place_node->key)
            find_place_node = find_place_node->right;
        else
            return std::make_pair(find_place_node, false); // Key already exist
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

template <typename Key>
void RBTree<Key>::insertFixup(Node* node) {
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

template <typename Key>
void RBTree<Key>::leftRotate(Node* node) {
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

template <typename Key>
void RBTree<Key>::rightRotate(Node* node) {
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

template <typename Key>
typename RBTree<Key>::Node* RBTree<Key>::minimum(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <typename Key>
typename RBTree<Key>::Node* RBTree<Key>::maximum(Node* node) const {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template <typename Key>
bool RBTree<Key>::contains(const Key& key) const {
    return findNode(key) != nullptr;
}

template <typename Key>
typename RBTree<Key>::size_type RBTree<Key>::size() const {
    return node_count;
}

template <typename Key>
void RBTree<Key>::erase(const Key& key) {
    Node* node = findNode(key);
    if (node == nullptr)
        return;
    Node* delete_node = node;
    Node* replace_node = nullptr;
    Color orig_color = delete_node->color;

    if (node->left == nullptr) { // Нет левого потомка
        replace_node = node->right;
        transplant(node, node->right);
    } else if (node->right == nullptr) { // Нет правого потомка
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

template <typename Key>
void RBTree<Key>::transplant(Node* first_node, Node* second_node) {
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

template <typename Key>
void RBTree<Key>::deleteFixup(Node* node) {
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

template <typename Key>
void RBTree<Key>::handleBrother(Node* node, Node* brother) {
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


template <typename Key>
RBTree<Key>& RBTree<Key>::operator=(const RBTree& other) {
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

template <typename Key>
RBTree<Key>& RBTree<Key>::operator=(RBTree&& other) noexcept {
    if (this != &other) {
        clear(root);
        root = other.root;
        node_count = other.node_count;

        other.root = nullptr;
        other.node_count = 0;
    }
    return *this;
}

template <typename Key>
typename RBTree<Key>::Node* RBTree<Key>::copySubtree(Node* node, Node* parent) {
    if (node == nullptr)
        return nullptr;
    
    Node* new_node = new Node(node->key);
    new_node->color = node->color;
    new_node->parent = parent;

    new_node->left = copySubtree(node->left, new_node);
    new_node->right = copySubtree(node->right, new_node);
    
    return new_node;
}

template <typename Key>
void RBTree<Key>::print() const {
    if (root == nullptr)
        std::cout << "Tree is empty!" << std::endl;
    else {
        char prefix[500] = "";
        printTree(root, prefix, true);
    }
}

template <typename Key>
void RBTree<Key>::printTree(Node* node, char prefix[], bool isLeft) const {
    if (node != nullptr) {
        char newPrefix[1000];
        strcpy(newPrefix, prefix);

        if (node->right) {
            strcat(newPrefix, (isLeft ? "│   " : "    "));
            printTree(node->right, newPrefix, false);
            strcpy(newPrefix, prefix);
        }
        std::cout << prefix << (isLeft ? "└── " : "┌── ") 
                  << node->key << (node->color == RED ? " (R)" : " (B)") << std::endl;
        strcpy(newPrefix, prefix);
        strcat(newPrefix, (isLeft ? "    " : "│   "));
        if (node->left) {
            printTree(node->left, newPrefix, true);
        }
    }
}