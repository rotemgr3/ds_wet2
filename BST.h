#ifndef BST_H_
#define BST_H_

#include <memory>
#include <assert.h>

template <class keyT, class dataT>
class Node {
    public:
        keyT key;
        std::shared_ptr<dataT> data;
        std::shared_ptr<Node<keyT, dataT>> left;
        std::shared_ptr<Node<keyT, dataT>> right; 
        int height;
        Node(const keyT& key, std::shared_ptr<dataT> data) : key(key), data(data), 
            left(nullptr), right(nullptr), height(0) {}
        ~Node() = default;
};

template <class keyT, class dataT>
class BST {
    private:
        std::shared_ptr<Node<keyT, dataT>> root;
        static void InOrderAux(BST<keyT, dataT> root, dataT* sorted_data,int n);
        static std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::InsertAux(std::shared_ptr<Node<keyT, dataT>> root, 
                                                            std::shared_ptr<Node<keyT, dataT>> toInsert); 
        static int GetHeight(std::shared_ptr<Node<keyT, dataT>> node);
        static int GetBF(std::shared_ptr<Node<keyT, dataT>> node);
        static std::shared_ptr<Node<keyT, dataT>> LLRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> LRRotation(std::shared_ptr<Node<keyT, dataT>> root)
        static std::shared_ptr<Node<keyT, dataT>> RLRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> RRRotation(std::shared_ptr<Node<keyT, dataT>> root);

    public:
        BST() : root(nullptr) {}
        BST(const BST<keyT, dataT>& tree);
        ~BST();
        std::shared_ptr<dataT> Get(const keyT& target);
        bool Find(const keyT& target);
        dataT* InOrderNElements(const int n);
        void Insert(const keyT& key,const dataT& data);
        void Remove(const keyT& key);
        BST<keyT, dataT> Merge(const BST<keyT, dataT>& tree_to_merge);
            
};

/*template <class keyT,class dataT>
BST<keyT, dataT>::BST(const BST<keyT, dataT>& tree)
{

}//later
*/

template <class keyT, class dataT>
std::shared_ptr<dataT> <BST<keyT, dataT>::Get(const keyT& target)
{
    std::shared_ptr<Node<keyT, dataT>> curr = root;
    while(curr != nullptr)
    {
        if(curr->key == target)
            return curr->data;
        if(curr->key < target)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return nullptr;
}

template <class keyT, class dataT>
bool BST<keyT, dataT>::Find(const keyT& target)
{
    if(this->Get(target) == nullptr)
        return false;
    return true;
}

template <class keyT, class dataT>
dataT* BST<keyT, dataT>::InOrderNElements(const int n)
{
    dataT* sorted_data = malloc(sizeof(*sorted_data) * n);
    if(sorted_data == nullptr)
        return nullptr;
    BST<keyT, dataT>::InOrderAux(this, sorted_data, n);
    return sorted_data;  
}

template <class keyT, class dataT>
void BST<keyT, dataT>::InOrderAux(BST<keyT, dataT> root, dataT* sorted_data,int n)
{
    if (root == nullptr)
        return;
    if (n == 0)
        return;
    BST<keyT, dataT>::InOrderAux(root->left, sorted_data, n);
    *sorted_data = dataT(*(root->data));
    BST<keyT, dataT>::InOrderAux(root->right, sorted_data + 1, n - 1);

    return;
}

template <class keyT, class dataT>
void BST<keyT, dataT>::Insert(const keyT& key,const dataT& data)
{
    if(this->Find(key))
        return;//handle it later
    
    std::shared_ptr<dataT> copyData = std::shared_ptr<dataT>(new dataT(data)); 
    auto toInsert = std::shared_ptr<Node<keyT, dataT>>(new Node<keyT, dataT>(key,copyData));
    
    BST<keyT, dataT>::InsertAux(this->root, toInsert);
}

template <class keyT, class dataT>
int BST<keyT, dataT>::GetHeight(std::shared_ptr<Node<keyT, dataT>> node)
{
    if(node == nullptr)
        return -1;
    return node->height;
}

template <class keyT, class dataT>
int BST<keyT, dataT>::GetBF(std::shared_ptr<Node<keyT, dataT>> node)
{
    return BST<keyT, dataT>::GetHeight(node->left) - BST<keyT, dataT>::GetHeight(node->right);
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::LLRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    std::shared_ptr<Node<keyT, dataT>> newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    newRoot->left = root->left->left;
    root->height = max(BST<keyT, dataT>::GetHeight(root->left), BST<keyT, dataT>::GetHeight(root->right)) + 1;
    newRoot->height = max(BST<keyT, dataT>::GetHeight(newRoot->left), BST<keyT, dataT>::GetHeight(newRoot->right)) + 1;
    return newRoot;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::LRRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RLRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RRRotation(std::shared_ptr<Node<keyT, dataT>> root)
{

}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::InsertAux(std::shared_ptr<Node<keyT, dataT>> root, 
                                                            std::shared_ptr<Node<keyT, dataT>> toInsert)
{
    if (root == nullptr) 
        return toInsert;
    
    if (root->key < toInsert->key)
        root->right = BST<keyT, dataT>::InsertAux(root->right, toInsert);
    else
        root->left = BST<keyT, dataT>::InsertAux(root->left, toInsert);
    
    root->height = max(GetHeight(root->left), GetHeight(root->right)) + 1;
    balanceFactor = GetBF(root);

    if (balanceFactor == 2) {
        if (BST<keyT, dataT>::GetBF(root->left) >= 0)
            return BST<keyT, dataT>::LLRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->left) == -1)
            return BST<keyT, dataT>::LRRotation(root);
    }
    else if (balanceFactor == -2) {
        if (BST<keyT, dataT>::GetBF(root->right) <= 0)
            return BST<keyT, dataT>::RRRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->right) == 1)
            return BST<keyT, dataT>::RLRotation(root);
    }  
    return root;
}

#endif /* BST_H */
