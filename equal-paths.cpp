#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) {
        return true;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return true;
    }

    if(root->right == nullptr){
        return equalPaths(root->left);
    }
    if(root->left == nullptr){
        return equalPaths(root->right);
    }

    int left_depth = depth(root->left);
    int right_depth = depth(root->right);
    if (left_depth != right_depth) {
        return false;
    }
    return equalPaths(root->left) && equalPaths(root->right);
    

}

static int depth(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    int l =depth(node->left);
    int r =depth(node->right);
    if (l>r){
        return l+1;
    }
    return r+1;

}