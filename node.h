#ifndef NODE_H_
#define NODE_H_

#include <memory>

template <class keyT, class dataT>
class Node {
    public:
        keyT key;
        std::shared_ptr<dataT> data;
        std::shared_ptr<Node<keyT, dataT>> left;
        std::shared_ptr<Node<keyT, dataT>> right; 
        int height;
        Node(int height) : key(), data(nullptr), 
            left(nullptr), right(nullptr), height(height) {}
        Node(const keyT& key, std::shared_ptr<dataT> data) : key(key), data(data), 
            left(nullptr), right(nullptr), height(0) {}
        ~Node() = default;
        Node(const Node<keyT, dataT>& node) : key(node.key), data(node.data), 
            left(node.left), right(node.right), height(node.height) {}
        static int NodeGetBF(std::shared_ptr<Node<keyT, dataT>> node);
        static int NodeGetHeight(std::shared_ptr<Node<keyT, dataT>> node);
        
};

template <class keyT, class dataT>
int Node<keyT, dataT>::NodeGetHeight(std::shared_ptr<Node<keyT, dataT>> node)
{
    if(node == nullptr)
        return -1;
    return node->height;
}

template <class keyT, class dataT>
int Node<keyT, dataT>::NodeGetBF(std::shared_ptr<Node<keyT, dataT>> node)
{
    return Node<keyT, dataT>::NodeGetHeight(node->left) - Node<keyT, dataT>::NodeGetHeight(node->right);
}

#endif /* NODE_H */