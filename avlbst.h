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
    AVLNode<Key, Value>* getRoot() const; 

protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    void leftRotate(AVLNode<Key, Value>* node); 
    void rightRotate(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* node, int diff);


};

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getRoot() const {
    return static_cast<AVLNode<Key, Value>*>(this->root_);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // override the bst insert function
    AVLNode<Key, Value>* root = getRoot();  

    // empty tree 
    if (!root) 
    {
        root = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        root->setBalance(0);  
        this->root_ = root; 
        return;
    }

    // insert with BST insert method
    AVLNode<Key, Value>* current = root;
    AVLNode<Key, Value>* parent = nullptr;
    
    while (current != nullptr) 
    {
        parent = current;
        if (new_item.first == current->getKey())
        {
            current->setValue(new_item.second);
            return;
        }
        else if (new_item.first < current->getKey()) 
        {
            current = current->getLeft();
        } 
        else 
        {
            current = current->getRight();
        } 
    }


    // once the key is found, create the new node to insert
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);


    // insert node into correct place in the tree

    if (newNode->getKey() < parent->getKey()) // tree is left heavy
    {
        parent->setLeft(newNode); 
    } 
    else 
    {
        parent->setRight(newNode);  // tree is right heavy
    }

    newNode->setBalance(0);


    // fix balance for the parent node 
    if (parent->getBalance() == -1 || parent->getBalance() == 1)
    {
        parent->setBalance(0);
        return;
    }
    else if (parent->getBalance() == 0)
    {
        if (newNode == parent->getLeft()) 
        {
            parent->setBalance(-1);
        } 
        else 
        {
            parent->setBalance(1);
        }
        insertFix(parent, newNode); 
    }
} 

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // find the node to remove by walking the tree
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (node != nullptr && key != node->getKey())
    {
        if (key < node->getKey())
        {
            node = node->getLeft();
        }
        else if (key > node->getKey())
        {
            node = node->getRight();
        }
    }

    // key is not in the tree
    if (node == nullptr) return;

    // node has two children
    if (node->getLeft() != nullptr && node->getRight() != nullptr)
    {
        // swap with predecessor 
        AVLNode<Key, Value>* predecessor_ = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
        nodeSwap(node, predecessor_);
    }

    // node has one child or no children
    parent = node->getParent(); 
    AVLNode<Key, Value>* child = nullptr;

    if (node->getLeft() != nullptr)
    {
        child = node->getLeft();
    }
    else if (node->getRight() != nullptr)
    {
        child = node->getRight();
    }

    // relink parent and child nodes
    if (child != nullptr)
    {
        child->setParent(parent);
    }

    int diff = 0; 
    if (node == this->root_)
    {
        this->root_ = child;
    }
    else if (parent != nullptr)
    {
        if (parent->getLeft() == node)
        {
            parent->setLeft(child);
            diff = 1;
        }
        else
        {
            parent->setRight(child);
            diff = -1; 
        }
    }
    
    // delete the node and update pointers
    node->setLeft(nullptr);
    node->setRight(nullptr);
    node->setParent(nullptr); 
    delete node;

    // patch the tree
    removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template <class Key, class Value> 
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* node)
{
  
    // left rotation: take right child and make it the parent
    // make original parent the new left child 
  
    if (node == nullptr) return; 

    AVLNode<Key, Value>* rightChild = node->getRight(); 
    if (rightChild == nullptr) return; 
    
    AVLNode<Key, Value>* parent = node->getParent(); 

    // update right subtree
    node->setRight(rightChild->getLeft()); 
    if (rightChild->getLeft()) 
    {
        rightChild->getLeft()->setParent(node); 
    }

    // promote the right child 
    rightChild->setParent(parent); 
    if (!parent) 
    {
        this->root_ = rightChild; 
    }
    else if (node == parent->getLeft())
    {
        parent->setLeft(rightChild);
    }
    else 
    {
        parent->setRight(rightChild); 
    }

    // update left subtree 
    rightChild->setLeft(node); 
    node->setParent(rightChild); 
}

template <class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* node)
{
    // right rotation: take left child, make it the parent
    // make original parent the new right child

    if (node == nullptr) return; 

    AVLNode<Key, Value>* leftChild = node->getLeft(); 
    if (leftChild == nullptr) return; 
    
    AVLNode<Key, Value>* parent = node->getParent(); 

    // update left subtree
    node->setLeft(leftChild->getRight()); 
    if (leftChild->getRight()) 
    {
        leftChild->getRight()->setParent(node); 
    }

    // promote the right child 
    leftChild->setParent(parent); 
    if (!parent) 
    {
        this->root_ = leftChild; 
    }
    else if (node == parent->getLeft())
    {
        parent->setLeft(leftChild);
    }
    else 
    {
        parent->setRight(leftChild); 
    }

    // update right subtree 
    leftChild->setRight(node); 
    node->setParent(leftChild); 

 } 

// fixes the AVL tree after inserting to maintain AVL balance property 
// precondition: p and n are balanced
// postcondition: g, p, n are balanced 
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    // parent or grandparent is null
    if (p == nullptr || p->getParent() == nullptr) return; 

    // g = grandparent
    AVLNode<Key, Value>* g = p->getParent(); 

    // assume p is left child of g
    if (g->getLeft() == p)
    {
        // update g's balance to new accurate value for now 
        g->updateBalance(-1); 

        // case 1: b(g) == 0, return
        if (g->getBalance() == 0) return; 

        // case 2: b(g) == -1, recurse 
        if (g->getBalance() == -1)
        {
            insertFix(g, p); 
            return;
        }

        // case 3: b(g) == -2
        if (g->getBalance() == -2)
        {
            // zig-zig (right right)
            if (p->getLeft() == n)
            {
                rightRotate(g); 
                p->setBalance(0);
                g->setBalance(0);
            }
        
            // zig-zag (left right) 
            else 
            {
                leftRotate(p); 
                rightRotate(g); 

                if (n->getBalance() == -1)
                {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 1) 
                {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    // assume p is right child of g
    else if (g->getRight() == p)
    {
        g->updateBalance(1);

        if (g->getBalance() == 0)
        {
            return; 
        }
        else if (g->getBalance() == 1)
        {
            insertFix(g,p);
            return;
        }
        else if (g->getBalance() == 2)
        {
            // zig zig (left left)
            if (p->getRight() == n)
            {
                leftRotate(g); 
                p->setBalance(0);
                g->setBalance(0);
            }
            // zig-zag (right left)
            else
            {
                rightRotate(p);
                leftRotate(g);
                
                if (n->getBalance() == 1)
                {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (n->getBalance() == -1)
                {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            } 
        }
    }
}


// fixes and rebalances the tree after a node is removed
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff)
{
    if (node == nullptr) 
    {
        return; 
    }

    // compute next recursive call arguments before altering the tree
    AVLNode<Key, Value>* p = node->getParent();
    int nextDiff = 0;

    if (p != nullptr)
    {
        if (node == p->getLeft())
        {
            // left child
            nextDiff = 1;
        }
        else
        {
            // right child 
            nextDiff = -1;
        }
    }
    
    int balance = node->getBalance() + diff;
    node->setBalance(balance);
    
    // case 1: b(n) + diff == -2
    if (balance == -2)
    {
        // let c = left(n), the taller of the children
        AVLNode<Key, Value>* c = node->getLeft();

        if (c == nullptr) return; 

        // case 1a: zig-zig (right right) 
        if (c->getBalance() == -1)
        {
            rightRotate(node);
            node->setBalance(0);
            c->setBalance(0);
            removeFix(p, nextDiff);
        }
        // case 1b: zig-zig (right) 
        else if (c->getBalance() == 0)
        {
            rightRotate(node);
            node->setBalance(-1);
            c->setBalance(1);
        }
        // case 1c: zig zag (left right)
        else 
        {
            AVLNode<Key, Value>* g = c->getRight(); 
            if (g == nullptr) return; 

            leftRotate(c); 
            rightRotate(node); 

                // subcase 1
                if (g->getBalance() == 1)
                {
                    node->setBalance(0); 
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                // subcase 2
                else if (g->getBalance() == 0)
                {
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else 
                {
                    node->setBalance(1); 
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, nextDiff); 
        }
    }
    // mirror case 1: b(n) + diff == 2
    else if (balance == 2)
    {
        // let c = right(n), the taller of the children
        AVLNode<Key, Value>* c = node->getRight();

        // case 1a: zig-zig (left left)
        if (c->getBalance() == 1)
        {
            leftRotate(node);
            node->setBalance(0);
            c->setBalance(0);
            removeFix(p, nextDiff);
        }
        // case 1b: zig-zig (left)
        else if (c->getBalance() == 0)
        {
            leftRotate(node);
            node->setBalance(1);
            c->setBalance(-1);
        }
        // case 1c: zig zag (right left) 
        else if (c->getBalance() == -1)
        {
            AVLNode<Key, Value>* g = c->getLeft(); 
            rightRotate(c); 
            leftRotate(node);

            // subcase 1
            if (g->getBalance() == -1)
            {
                node->setBalance(0); 
                c->setBalance(1);
                g->setBalance(0);
            }
            // subcase 2
            else if (g->getBalance() == 0)
            {
                node->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            else if (g->getBalance() == 1)
            {
                node->setBalance(-1); 
                c->setBalance(0);
                g->setBalance(0);
            }
           removeFix(p, nextDiff); 
        }   
    }
    // case 2: b(n) + diff == -1, done
    else if (balance == -1)
    {
        node->setBalance(-1); 
    }
    else if (balance == 1)
    {
        node->setBalance(1);
    }
    // case 3: b(n) + diff == 0, recurse
    else if (balance == 0)
    {
        node->setBalance(0); 
        removeFix(p, nextDiff);
    }
}

#endif
