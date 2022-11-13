#include "diameter.h"
#include <algorithm>

int diaRecur(const BinaryTreeNode* root, int& d) {
  if (!root) return 0;
  int leftD = diaRecur(root->left, d);
  int rightD = diaRecur(root->right, d);
  d = std::max(d, leftD + rightD);
  return std::max(leftD, rightD) + 1;
}

int diameter(const BinaryTreeNode* tree) {
  int d = 0;
  diaRecur(tree, d);
  return d;
}