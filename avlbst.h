#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);




};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        } else if (new_item.first > current->getKey()) {
            current = current->getRight();
        } else {
            current->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first<parent->getKey()) {
        parent->setLeft(newNode);
        parent->updateBalance(-1);
        if (parent->getBalance()!=0){ 
        insertFix(parent, newNode);
        }
    }else{
        parent->setRight(newNode);
        parent->updateBalance(1);
        if (parent->getBalance()!=0){ 
            insertFix(parent, newNode);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* grand = parent->getParent();
    if (grand == nullptr) {
        return;
    }

    if (parent == grand->getLeft()) {
        grand->updateBalance(-1);

        if (grand->getBalance() == 0) {
            return;
        }
        else if (grand->getBalance() == -1) {
            insertFix(grand, parent);
        }
        else if (grand->getBalance() == -2) {
            if (node == parent->getLeft()) {
                rotateRight(grand);
                parent->setBalance(0);
                grand->setBalance(0);
            }
            else {
                AVLNode<Key, Value>* child = node;
                int8_t cb = child->getBalance();

                rotateLeft(parent);
                rotateRight(grand);

                if (cb == -1) {
                    parent->setBalance(0);
                    grand->setBalance(1);
                }
                else if (cb == 0) {
                    parent->setBalance(0);
                    grand->setBalance(0);
                }
                else { 
                    parent->setBalance(-1);
                    grand->setBalance(0);
                }
                child->setBalance(0);
            }
        }
    }
    else { 
        grand->updateBalance(1);

        if (grand->getBalance() == 0) {
            return;
        }
        else if (grand->getBalance() == 1) {
            insertFix(grand, parent);
        }
        else if (grand->getBalance() == 2) {
            if (node == parent->getRight()) {
                rotateLeft(grand);
                parent->setBalance(0);
                grand->setBalance(0);
            }
            else {
                AVLNode<Key, Value>* child = node;
                int8_t cb = child->getBalance();

                rotateRight(parent);
                rotateLeft(grand);

                if (cb == 1) {
                    parent->setBalance(0);
                    grand->setBalance(-1);
                }
                else if (cb == 0) {
                    parent->setBalance(0);
                    grand->setBalance(0);
                }
                else { 
                    parent->setBalance(1);
                    grand->setBalance(0);
                }
                child->setBalance(0);
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (nodeToRemove == nullptr) {
        return;
    }

    if (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, pred);
    }

    AVLNode<Key, Value>* child;
    if (nodeToRemove->getLeft() != nullptr) {
        child = nodeToRemove->getLeft();
    } else {
        child = nodeToRemove->getRight();
    }

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();
    int8_t balanceDiff = 0;

    if (parent!=NULL){
        if (parent->getLeft() == nodeToRemove) {
            balanceDiff = 1;  
        } else {
            balanceDiff = -1; 
        }
    }

    if (child != nullptr) {
        child->setParent(parent);
    }

    if (parent == nullptr) {
        this->root_ = child;
    } else {
        if (parent->getLeft() == nodeToRemove) {
        parent->setLeft(child);
        } else {
        parent->setRight(child);
        }
    }
    delete nodeToRemove;

    if (parent != nullptr) {
        removeFix(parent, balanceDiff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
    AVLNode<Key, Value>* n = node;
    int8_t d = diff;

    while (n != nullptr) {
        n->updateBalance(d);
        int8_t bal = n->getBalance();

        AVLNode<Key, Value>* p = n->getParent();
        int8_t nextDiff = 0;
        if (p != nullptr) {
            if (p->getLeft() == n) {
                nextDiff = 1;   
            }
            else {
                nextDiff = -1;  
            }
        }

        if (bal == -2) {
            AVLNode<Key, Value>* y = n->getLeft();
            if (y->getBalance() <= 0) {
                rotateRight(n);
                if (y->getBalance() == 0) {
                    n->setBalance(-1);
                    y->setBalance(1);
                    break; 
                }
                else {
                    n->setBalance(0);
                    y->setBalance(0);
                    n = p;
                    d = nextDiff;
                    continue;
                }
            }
            else {
                AVLNode<Key, Value>* x = y->getRight();
                int8_t xb = x->getBalance();

                rotateLeft(y);
                rotateRight(n);

                if (xb == -1) {
                    n->setBalance(1);
                    y->setBalance(0);
                }
                else if (xb == 0) {
                    n->setBalance(0);
                    y->setBalance(0);
                }
                else { 
                    n->setBalance(0);
                    y->setBalance(-1);
                }
                x->setBalance(0);

                n = p;
                d = nextDiff;
                continue;
            }
        }
        else if (bal == 2) {
            AVLNode<Key, Value>* y = n->getRight();
            if (y->getBalance() >= 0) {
                rotateLeft(n);
                if (y->getBalance() == 0) {
                    n->setBalance(1);
                    y->setBalance(-1);
                    break; 
                }
                else {
                    n->setBalance(0);
                    y->setBalance(0);
                    n = p;
                    d = nextDiff;
                    continue;
                }
            }
            else {
                AVLNode<Key, Value>* x = y->getLeft();
                int8_t xb = x->getBalance();

                rotateRight(y);
                rotateLeft(n);

                if (xb == 1) {
                    n->setBalance(-1);
                    y->setBalance(0);
                }
                else if (xb == 0) {
                    n->setBalance(0);
                    y->setBalance(0);
                }
                else { 
                    n->setBalance(0);
                    y->setBalance(1);
                }
                x->setBalance(0);

                n = p;
                d = nextDiff;
                continue;
            }
        }
        else if (bal == 0) {
            n = p;
            d = nextDiff;
            continue;
        }
        else { 
            break;
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* rightChild = node->getRight();
    if (rightChild == nullptr) {
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* rightLeftChild = rightChild->getLeft();

    rightChild->setParent(parent);
    if (parent == nullptr) {
        this->root_ = rightChild;
    } else if (parent->getLeft() == node) {
        parent->setLeft(rightChild);
    } else {
        parent->setRight(rightChild);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);
    node->setRight(rightLeftChild);
    if (rightLeftChild != nullptr) {
        rightLeftChild->setParent(node);
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* leftChild = node->getLeft();
    if (leftChild == nullptr) {
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* leftRightChild = leftChild->getRight();

    leftChild->setParent(parent);
    if (parent == nullptr) {
        this->root_ = leftChild;
    } else if (parent->getLeft() == node) {
        parent->setLeft(leftChild);
    } else {
        parent->setRight(leftChild);
    }

    leftChild->setRight(node);
    node->setParent(leftChild);
    node->setLeft(leftRightChild);
    if (leftRightChild != nullptr) {
        leftRightChild->setParent(node);
    }
}

#endif
