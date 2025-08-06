#ifndef BTREE_H  
#define BTREE_H 

#include <vector>
#include <iostream>
#include <string> 
#include <algorithm>
#include "bTreeNode.h"

template<typename T>
bool isGreater(const T& a, const T& b);

template<typename T>
bool isEqual(const T& a, const T& b);

template <typename T>
class Btree {
private:
    BtreeNode<T>* root;
    int t;

public:
    Btree(int t);

    void insert(const T& data, int blocNum);

    std::vector<std::pair<T, std::vector<int>>> getRootNames();

    BtreeNode<T>* getRoot();

    std::vector<int> getBlockNum(T val);

    void printTree(BtreeNode<T>* node, const std::string& prefix = "", bool isLast = true);
};


#include "bTree.cpp"

#endif
