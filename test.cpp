
#include <stdlib.h>
#include <iostream>
#include "BST.h"
using namespace std;

void preOrder(std::shared_ptr<Node<int, int>> root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Driver Code
int main()
{
    BST<int, int> tree = BST<int, int>();
    
    /* Constructing tree given in
    the above figure */
    tree.Insert(10,1);
    tree.Insert(20,2);
    tree.Insert(30,3);
    tree.Insert(40,4);
    tree.Insert(50,5);
    tree.Insert(25,6);
    /* The constructed AVL Tree would be
            30
            / \
            20 40
            / \ \
           10 25 50
    */
    std::cout << "Preorder traversal of the "
            "constructed AVL tree is \n";
    preOrder(tree.root);
     
    return 0;
}

/* The constructed AVL Tree would be
            20
            / \
           10 30         
*/
/* The constructed AVL Tree would be
            10
            / \
              20
                \
                 30         
*/
