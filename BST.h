#ifndef BST_H_
#define BST_H_

#include <memory>
#include "node.h"

int max(int a, int b)
{
    return (a > b)? a : b;
}

template <class keyT, class dataT>
class BST {
    private:
        static std::shared_ptr<Node<keyT, dataT>> InsertAux(std::shared_ptr<Node<keyT, dataT>> root, 
                                                            std::shared_ptr<Node<keyT, dataT>> toInsert); 
        static int GetBF(std::shared_ptr<Node<keyT, dataT>> node);
        static int GetHeight(std::shared_ptr<Node<keyT, dataT>> node);
        static std::shared_ptr<Node<keyT, dataT>> LLRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> LRRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> RLRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> RRRotation(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> RemoveAux(std::shared_ptr<Node<keyT, dataT>> root, const keyT& key);
        static std::shared_ptr<Node<keyT, dataT>> RemoveAlgo(std::shared_ptr<Node<keyT, dataT>> root);
        static std::shared_ptr<Node<keyT, dataT>> FindNextInOrder(std::shared_ptr<Node<keyT, dataT>> root);


    public:
        std::shared_ptr<Node<keyT, dataT>> root;
        int size;

        BST() : root(nullptr), size(0) {}
        ~BST() = default;
        std::shared_ptr<dataT> Get(const keyT& target);
        bool Find(const keyT& target);
        void Insert(const keyT& key,const dataT& data);
        void Remove(const keyT& key);
        BST<keyT, dataT> Merge(const BST<keyT, dataT>& tree_to_merge);
        dataT& GetMax();
        dataT& GetMin();
            
};

template <class keyT, class dataT>
std::shared_ptr<dataT> BST<keyT, dataT>::Get(const keyT& target)
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
void BST<keyT, dataT>::Insert(const keyT& key,const dataT& data)
{
    if(this->Find(key))
        return;//handle it later
    
    std::shared_ptr<dataT> copyData = std::shared_ptr<dataT>(&data)); //changed from new dataT(data)
    auto toInsert = std::shared_ptr<Node<keyT, dataT>>(new Node<keyT, dataT>(key,copyData));

    this->root = BST<keyT, dataT>::InsertAux(this->root, toInsert);
    this->size++;
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
    std::shared_ptr<Node<keyT, dataT>> B = root;
    std::shared_ptr<Node<keyT, dataT>> A = B->left;
    std::shared_ptr<Node<keyT, dataT>> AL = A->left;
    std::shared_ptr<Node<keyT, dataT>> AR = A->right;

    B->left = AR;
    A->right = B;
    
    B->height = max(GetHeight(B->left), GetHeight(B->right)) + 1;
    A->height = max(GetHeight(A->left), GetHeight(A->right)) + 1;   
     
    return A;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::LRRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    std::shared_ptr<Node<keyT, dataT>> A = root->left;
    std::shared_ptr<Node<keyT, dataT>> B = A->right;
    std::shared_ptr<Node<keyT, dataT>> C = root;
    std::shared_ptr<Node<keyT, dataT>> BL = B->left;
    std::shared_ptr<Node<keyT, dataT>> BR = B->right;

    C->left = BR;
    A->right = BL;
    B->left = A;
    B->right = C;

    A->height = max(GetHeight(A->left), GetHeight(A->right)) + 1;
    C->height = max(GetHeight(C->left), GetHeight(C->right)) + 1;
    B->height = max(GetHeight(B->left), GetHeight(B->right)) + 1;
    
    return B;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RLRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    std::shared_ptr<Node<keyT, dataT>> A = root->right;
    std::shared_ptr<Node<keyT, dataT>> B = A->left;
    std::shared_ptr<Node<keyT, dataT>> C = root;
    std::shared_ptr<Node<keyT, dataT>> BL = B->left;
    std::shared_ptr<Node<keyT, dataT>> BR = B->right;

    C->right = BL;
    A->left = BR;
    B->right = A;
    B->left = C;

    A->height = max(GetHeight(A->left), GetHeight(A->right)) + 1;
    C->height = max(GetHeight(C->left), GetHeight(C->right)) + 1;
    B->height = max(GetHeight(B->left), GetHeight(B->right)) + 1;

    return B;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RRRotation(std::shared_ptr<Node<keyT, dataT>> root)
{
    std::shared_ptr<Node<keyT, dataT>> B = root;
    std::shared_ptr<Node<keyT, dataT>> A = B->right;
    std::shared_ptr<Node<keyT, dataT>> AL = A->left;
    std::shared_ptr<Node<keyT, dataT>> AR = A->right;

    B->right = AL;
    A->left = B;
    
    B->height = max(GetHeight(B->left), GetHeight(B->right)) + 1;
    A->height = max(GetHeight(A->left), GetHeight(A->right)) + 1;
    
    return A;
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
    int balanceFactor = GetBF(root);

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

template <class keyT, class dataT>
void BST<keyT, dataT>::Remove(const keyT& key)
{
    if(!this->Find(key))
        return;//handle it later
    
    this->root = BST<keyT, dataT>::RemoveAux(this->root, key);
    this->size--;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RemoveAux(std::shared_ptr<Node<keyT, dataT>> root, const keyT& key)
{
    if (root->key == key) 
        return BST<keyT, dataT>::RemoveAlgo(root);
    
    if (root->key < key)
        root->right = BST<keyT, dataT>::RemoveAux(root->right, key);
    else
        root->left = BST<keyT, dataT>::RemoveAux(root->left, key);
    
    root->height = max(GetHeight(root->left), GetHeight(root->right)) + 1;
    int balanceFactor = GetBF(root);

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

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RemoveAlgo(std::shared_ptr<Node<keyT, dataT>> root)
{
    if (!root->left && !root->right)
        return nullptr;
    
    if (root->left && root->right) {
        std::shared_ptr<Node<keyT, dataT>> nextInOrder = BST<keyT, dataT>::FindNextInOrder(root);
        std::shared_ptr<Node<keyT, dataT>> temp = root;
        root = nextInOrder;
        nextInOrder = temp;
        return BST<keyT, dataT>::RemoveAlgo(root);
    }
    if(root->left)
        return root->left;
   
    return root->right; 
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::FindNextInOrder(std::shared_ptr<Node<keyT, dataT>> root)
{
    root = root->right;
    while(root->left != nullptr)
        root = root->left;

    return root;
}

template <class keyT, class dataT>
BST<keyT, dataT> BST<keyT, dataT>::Merge(const BST<keyT, dataT>& tree_to_merge)
{
    
}

template <class keyT, class dataT>
dataT& BST<keyT, dataT>::GetMax()
{
    std::shared_ptr<Node<keyT, dataT>> curr = this->root;
    while(curr->right != nullptr)
        curr = curr->right;

    return *(curr->data);
}

template <class keyT, class dataT>
dataT& BST<keyT, dataT>::GetMin()
{
    std::shared_ptr<Node<keyT, dataT>> curr = this->root;
    while(curr->left != nullptr)
        curr = curr->left;

    return *(curr->data);
}

#endif /* BST_H */
