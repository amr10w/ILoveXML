#include "Tree.h"

template<typename T>
Tree<T>::Tree() {
    root = nullptr;
}

template <typename T>
Tree<T>::Tree(Node<T> *root):root(root)
{

}

template <typename T>
Tree<T>::Tree(const Tree<T> &other)
{
    root = copy(other.root);
}

template<typename T>
Tree<T>::~Tree() {
    destroy(root);
}


// deep copy
template <typename T>
Node<T> *Tree<T>::copy(Node<T> *item)
{
    if (!n) return nullptr;
    Node<T>* node = new Node<T>();
    node->data = item->data;
    for (auto e : item->children)
        node->children.push_back(copy(e));
    return node;
}

template <typename T>
void Tree<T>::destroy(Node<T> *node)
{
    if (!node) return;
        for (auto e : node->children) destroy(e);
    delete node;
}