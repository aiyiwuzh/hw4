#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;

// helper func to check if all leaf-to-root paths have the same length
bool equalPathsHelper(Node* root, int depth, int& leafDepth) 
{
    // base case: empty tree
    if (root == nullptr) {
        return true;
    }

    // base case: leaf node 
    if (root->left == nullptr && root->right == nullptr) 
    {
        if (leafDepth == -1) 
        {  
            leafDepth = depth; 
            return true;
        }
        return depth == leafDepth; 
    }

    // recurse for left and right subtrees
    return equalPathsHelper(root->left, depth + 1, leafDepth) &&
           equalPathsHelper(root->right, depth + 1, leafDepth);
}

bool equalPaths(Node* root) {
    int leafDepth = -1;  
    return equalPathsHelper(root, 0, leafDepth);
}

