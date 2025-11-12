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
    void insertRestore(AVLNode<Key, Value>* parent, AVLNode<Key,Value>* node);
    void removeRestore(AVLNode<Key, Value>* parent, int diff);
    void LeftRotate(AVLNode<Key,Value>* node);
    void RightRotate(AVLNode<Key,Value>* node);
    

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
        this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key,Value>* curr = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* parent = NULL;

    while(curr != NULL){
        parent = curr;
        if(new_item.first < curr->getKey()){
            curr = curr->getLeft();

        } else if (new_item.first > curr->getKey()){
            curr = curr->getRight();
        } else {
            curr->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key,Value>* node = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);

    if(new_item.first < parent->getKey()){
        parent->setLeft(node);
    } else {
        parent->setRight(node);
    }

    if(parent->getBalance() == -1 || parent->getBalance() == 1){
        parent->setBalance(0);
    } else{
        if(parent->getLeft() == node){
            parent->setBalance(-1);
        } else {
            parent->setBalance(1);
        }
        insertRestore(parent, node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertRestore(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node){
    if(parent == NULL || parent->getParent() == NULL){
        return;
    } 

    AVLNode<Key,Value>* grandparent = parent->getParent();

    if(grandparent->getLeft() == parent){
        grandparent->updateBalance(-1);

        if(grandparent->getBalance() == 0){
            return;
        } else if(grandparent->getBalance() == -1){
            insertRestore(grandparent, parent);
        } else{
            if(parent->getLeft() == node){
                RightRotate(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                LeftRotate(parent);
                RightRotate(grandparent);

                if(node->getBalance() == -1){
                    parent->setBalance(0);
                    grandparent->setBalance(1);  
                }
                 else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }
                node->setBalance(0);
            }
        }
    } else {
        grandparent->updateBalance(1);
        
        if(grandparent->getBalance() == 0){
            return;
        } else if(grandparent->getBalance()== 1){
            insertRestore(grandparent, parent);
        } else{
            if(parent->getRight() == node){
                LeftRotate(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else{
                RightRotate(parent);
                LeftRotate(grandparent);
                
                if(node->getBalance() == 1){
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                } else if( node->getBalance() == 0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else{
                    parent->setBalance(1);
                    grandparent->setBalance(0);

                }
                node->setBalance(0);
            }
        }
    }
}


template<class Key, class Value>
void AVLTree<Key,Value>::LeftRotate(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();


    node->setRight(right->getLeft());
    if(right->getLeft() != NULL){
        right->getLeft()->setParent(node);
    }   

    right->setLeft(node);
    node->setParent(right);

    right->setParent(parent);
    if(parent == NULL){
        this->root_ = right;
    } else if (parent->getLeft() == node){
        parent->setLeft(right);
    } else{
        parent->setRight(right);
    }

}

template<class Key, class Value>
void AVLTree<Key,Value>::RightRotate(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();


    node->setLeft(left->getRight());
    if(left->getRight() != NULL){
        left->getRight()->setParent(node);
    }   

    left->setRight(node);
    node->setParent(left);

    left->setParent(parent);
    if(parent == NULL){
        this->root_ = left;
    } else if (parent->getLeft() == node){
        parent->setLeft(left);
    } else{
        parent->setRight(left);
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if (node == NULL){
        return;
    }

    if(node->getLeft() != NULL && node->getRight() != NULL){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::predecessor(node));
        nodeSwap(node, pred);
    }

    AVLNode<Key,Value>* parent = node->getParent();
    int diff = 0;
    if(parent != NULL){
        if(parent->getLeft() == node){
            diff = 1;
        } else {
            diff = -1;
        }
    }

    AVLNode<Key, Value>* child = NULL;
    if(node->getLeft() != NULL){
        child = node->getLeft();
    } else if(node->getRight() != NULL){
        child = node->getRight();
    }

    if(parent == NULL){
        this->root_ = child;
    } else {
        if(parent->getLeft() == node){
            parent->setLeft(child);
        } else{
            parent->setRight(child);
        }
    }

    if(child != NULL){
        child->setParent(parent);
    }

    delete node;

    removeRestore(parent, diff);
    // TODO
}


template<class Key, class Value>
void AVLTree<Key,Value>::removeRestore(AVLNode<Key,Value>* node, int diff){
    if(node == NULL){
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int ndiff = 0;
    if(parent != NULL){
        if(parent->getLeft() == node){
            ndiff = 1;

        } else{
            ndiff = -1;
        }
    }

    if(diff == -1){
        if(node->getBalance() + diff == -2){
            AVLNode<Key, Value>* left = node->getLeft();

            if(left->getBalance() == -1){
                RightRotate(node);
                node->setBalance(0);
                left->setBalance(0);
                removeRestore(parent, ndiff);
            } else if(left->getBalance() == 0){
                RightRotate(node);
                node->setBalance(-1);
                left->setBalance(1);
            } else{
                AVLNode<Key, Value>* grandchild = left->getRight();
                LeftRotate(left);
                RightRotate(node);

                if(grandchild->getBalance() == 1){
                    node->setBalance(0);
                    left->setBalance(-1);
                } else if(grandchild->getBalance() == 0){
                    node->setBalance(0);
                    left->setBalance(0);
                } else{
                    node->setBalance(1);
                    left->setBalance(0);
                
                }
                grandchild->setBalance(0);
                removeRestore(parent, ndiff);
            }
        }

        else if(node->getBalance() + diff == -1){
            node->setBalance(-1);
        } else {
            node->setBalance(0);
            removeRestore(parent, ndiff);
        }
    }
    else {
        if(node->getBalance() + diff == 2){
            AVLNode<Key, Value>* right = node->getRight();


            if(right->getBalance() == 1){
                LeftRotate(node);
                node->setBalance(0);
                right->setBalance(0);
                removeRestore(parent, ndiff);
            } else if(right->getBalance() == 0){
                LeftRotate(node);
                node->setBalance(1);
                right->setBalance(-1);
            } else{
                AVLNode<Key, Value>* grandchild = right->getLeft();
                RightRotate(right);
                LeftRotate(node);

                if(grandchild->getBalance() == -1){
                    node->setBalance(0);
                    right->setBalance(1);
                } else if(grandchild->getBalance() == 0){
                    node->setBalance(0);
                    right->setBalance(0);
                } else {
                    node->setBalance(-1);
                    right->setBalance(0);
                }
                grandchild->setBalance(0);
                removeRestore(parent, ndiff);

            }
        }
        else if(node->getBalance() + diff == 1){
            node->setBalance(1);

        } else{
            node->setBalance(0);
            removeRestore(parent, ndiff);
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


#endif
