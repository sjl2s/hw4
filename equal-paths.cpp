#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkEqualPaths(Node* node, int curr, int& leaf);

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){
      return true;
    }

    int leaf = -1;
    return checkEqualPaths(root, 0, leaf);

}

bool checkEqualPaths(Node* node, int curr, int& leaf){
  if(node->left == nullptr && node->right == nullptr){
    if(leaf == -1){
      leaf = curr;
      return true;
    }
    return curr == leaf;
  }
  bool left = true;
  bool right = true;

  if (node->left != nullptr){
    left = checkEqualPaths(node->left, curr+1, leaf);
  }
  if (node->right != nullptr){
    right = checkEqualPaths(node->right, curr+1, leaf);
  }

  return left && right;
}

