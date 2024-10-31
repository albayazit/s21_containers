#include "s21_tree.h"
#include <iostream>

int main() {
    RBTree<int> myTree;
    myTree.insert(3);
    myTree.insert(2);
    myTree.insert(1);
    myTree.insert(123);
    myTree.insert(12123);
    myTree.insert(12);
    myTree.insert(5);
    myTree.insert(0);
    myTree.erase(12);
    myTree.print();
    std::cout << myTree.contains(12);
    return 0;
}