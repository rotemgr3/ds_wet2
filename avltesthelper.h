
#ifndef AVL_TEST_HELPER_H
#define AVL_TEST_HELPER_H

#include <algorithm>
#include <cmath>
#include <assert.h>
#include <memory>

#define DBG 0

using std::max;

#include <iostream>
using std::cout;
using std::endl;

// A class template to test some of the internals of the AVL tree implementation.
// Is declared friend of BST so has access to these.
template<typename K, typename V>
class test_helper
{
private:  
  test_helper() {} // instantiation not needed (or allowed)
public:

  // Count the total number of nodes in the tree.
  static unsigned int count_nodes(const BST<K, V>& tree)
  {
    if (tree.root)
      return count_descendants(tree.root);
    else
      return 0u;
  }

  // Check the tree is AVL.
  static bool is_avl(const BST<K, V>& tree)
  {
    return is_avl(tree.root);
  }

  // Check the tree's balance factors are correct.
  static bool valid_balance_factors(const BST<K, V>& tree)
  {
    return valid_balance_factors(tree.root);
  }

  // Test the functions in this class.
  static void test_meta_functions()
  {
    {
      // Counting nodes
      BST<int, double> tree;
      assert(count_nodes(tree) == 0);  // Empty tree should have 0 nodes.
      tree.Insert(1, 1.0);
      assert(count_nodes(tree) == 1);  // one node inserted
      tree.Insert(1, 1.0);
      assert(count_nodes(tree) == 1);  // same node inserted twice
      tree.Insert(2, 2.0);
      assert(count_nodes(tree) == 2);  // second unique node inserted
      tree.Remove(3);
      assert(count_nodes(tree) == 2);  // non existent node removed
      tree.Remove(1);
      assert(count_nodes(tree) == 1);  // one node removed
      tree.Remove(2);
      assert(count_nodes(tree) == 0);
    }
    {
      // subtree height
      BST<int, double> tree;
      assert(subtree_height(tree.root) == 0);  // root is null, height is 0.
      tree.Insert(5, 5.0);
      assert(subtree_height(tree.root) == 1);  // single node height should be 1.
      tree.Insert(8, 8.0);
      assert(subtree_height(tree.root) == 2);  // height should be 2; root has a single right child
      tree.Insert(3, 3.0);
      assert(subtree_height(tree.root) == 2);  // height should be 2; root has a left and right child
      tree.Insert(7, 7.0);
      assert(subtree_height(tree.root) == 3);  // height should be 2; root has a left and right child    
    }
  }

private:
  static bool is_avl(const std::shared_ptr<Node<K, V>> root_node)
  {
    if (!root_node) return true;  // Base case; an empty tree is always AVL.
    bool left_is_avl, right_is_avl;
    unsigned int left_height, right_height;
    left_is_avl = is_avl(root_node->left);
    left_height = subtree_height(root_node->left);
    right_is_avl = is_avl(root_node->right);
    right_height = subtree_height(root_node->right);
    if (DBG) { cout << "left subtree: " << left_height << " | right subtree: " << right_height << endl; }
    if (DBG) { cout << "left_is_avl: " << left_is_avl << " | right_is_avl: " << right_is_avl << endl; }
    return left_is_avl && right_is_avl && (abs(left_height - right_height) <= 1u);
  }

  static bool valid_balance_factors(const std::shared_ptr<Node<K, V>> node)
  {
    if (!node) return true;
    if (DBG) { cout << "left_height: " << subtree_height(node->left) << " | right_height: "
         << subtree_height(node->right) << " | balance_factor: " << static_cast<int>(Node<K, V>::NodeGetBF(node)) << endl; }
    return (subtree_height(node->left) - subtree_height(node->right) == Node<K, V>::NodeGetBF(node))
            && valid_balance_factors(node->left) && valid_balance_factors(node->right);
  }

  static unsigned int count_descendants(const std::shared_ptr<Node<K, V>> node)
  {
    unsigned int left_count = node->left ? count_descendants(node->left) : 0;
    unsigned int right_count = node->right ? count_descendants(node->right) : 0;
    return 1u + left_count + right_count;
  }

  static unsigned int subtree_height(const std::shared_ptr<Node<K, V>> node)
  {
    if (!node) return 0u;  // no subtree root -> height = 0.
    unsigned int left_height, right_height;
    left_height = right_height = 0u;
    left_height = subtree_height(node->left);
    right_height = subtree_height(node->right);
    return 1 + max(left_height, right_height);
  }
};

#endif