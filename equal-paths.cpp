#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool equalPaths(Node * root)
{
    // base case 1: if root is null 
    if (root == nullptr)
    {
        return true;
    }

    // base case 2: root is the only node (leaf node)
    if (root->left == nullptr && root->right == nullptr)
    {
        return true; 
    }
    
    // root has only one subtree 
    if (root->left == nullptr)
    {
        return equalPaths(root->right);
    }
    if (root->right == nullptr)
    {
        return equalPaths(root->left);
    }

    // root has multiple subtrees 
    Node* l = root->left; 
    Node* r = root->right; 
    int l_height = 0, r_height = 0; 

    // check the left subtree
    while (l != nullptr)
    {
        l_height++;
        // move onto next node to check and increment the height
        if (l->left != nullptr)
        {
            l = l->left;
        }
        else 
        {
            l = l->right; 
        }
    }

    // check the right subtree
    while (r != nullptr)
    {
        r_height++; 
        if (r->left != nullptr) 
        {
            r = r->left;
        }
        else 
        {
            r = r->right;
        }
    }

    bool left_check = equalPaths(root->left); 
    bool right_check = equalPaths(root->right); 
    
    if ( (l_height == r_height) && left_check && right_check)
    {
        return true; 
    }
    else 
    {
        return false; 
    }
}

