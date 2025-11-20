#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>

// --- Node Structure (Struct) ---
template<typename T>
struct Node {
    T data {};
    
    std::vector<Node<T>*> children;
};

// --- Tree Class (Class) ---
template<typename T>
class Tree {
public:
    Tree();
    Tree(Node<T>* root);
    Tree(const Tree<T>& other); // deep copy (copy constructor)
    ~Tree();
private:
    Node<T>* root {};
    Node<T>* copy(Node<T>* n);
    void destroy(Node<T>* n);(Node<T>* n);
};

#endif // TREE_H