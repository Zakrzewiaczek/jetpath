#ifndef BTREENODE_CPP
#define BTREENODE_CPP

#include "bTreeNode.h"

template <typename T>
BtreeNode<T>::BtreeNode(int t) {
    this->t = t;
}

template <typename T>
std::vector<std::pair<T, std::vector<int>>> BtreeNode<T>::getNames() {
    return names;
}

template <typename T>
void BtreeNode<T>::replaceChild(int i, BtreeNode<T>* child) {
    if (i < childrens.size()) {
        childrens[i] = child;
    }
    else {
        std::cout << "Index out of bounds" << std::endl;
    }
}

template <typename T>
std::vector<BtreeNode<T>*> BtreeNode<T>::getChildrens() {
    return childrens;
}

template <typename T>
T BtreeNode<T>::getName() {
    return names[0].first;
}

template <typename T>
void BtreeNode<T>::addChildAtPos(int i, BtreeNode<T>* child) {
    childrens.insert(childrens.begin() + i, child);
}

template <typename T>
void BtreeNode<T>::addChild(BtreeNode<T>* child) {
    childrens.push_back(child);
}

template <typename T>
void BtreeNode<T>::addName(T name, int dataBlock) {
    bool isAlready = false;
    for (size_t i = 0; i < names.size(); i++) {
        if (names[i].first == name) {
            isAlready = true;
            names[i].second.push_back(dataBlock);
            break;
        }
    }
    if (isAlready == false) {
        std::vector<int> blocks = { dataBlock };
        names.push_back({ name, blocks });
    }
    std::sort(names.begin(), names.end(), [](const std::pair<T, std::vector<int>>& a, const std::pair<T, std::vector<int>>& b) {
        return a.first < b.first;
        });
}

template <typename T>
void BtreeNode<T>::insert(T data, int blockNum, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root) {
    traverse(path, root, data, blockNum);
}

template <typename T>
std::pair<T, std::vector<int>> BtreeNode<T>::splitNode(BtreeNode<T>* node, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root) {
    BtreeNode<T>* b1 = new BtreeNode<T>(t);
    BtreeNode<T>* b2 = new BtreeNode<T>(t);
    std::vector<std::pair<T, std::vector<int>>> tmpNames = node->getNames();

    for (size_t i = 0; i < node->getNames().size(); i++) {
        if (i < node->getNames().size() / 2) {
            for (int blockNum : tmpNames[i].second) {
                b1->addName(tmpNames[i].first, blockNum);
            }
        }
        else if (i > node->getNames().size() / 2) {
            for (int blockNum : tmpNames[i].second) {
                b2->addName(tmpNames[i].first, blockNum);
            }
        }
    }

    auto tmpChildrens = node->getChildrens();
    if (!tmpChildrens.empty()) {
        for (size_t j = 0; j <= b1->getNames().size(); j++) {
            if (j < tmpChildrens.size()) {
                b1->addChild(tmpChildrens[j]);
            }
        }

        for (size_t j = b1->getNames().size() + 1; j < tmpChildrens.size(); j++) {
            b2->addChild(tmpChildrens[j]);
        }
    }

    BtreeNode<T>* middle = new BtreeNode<T>(t);
    T middleValue = tmpNames[node->getNames().size() / 2].first;
    std::vector<int> middleBlocks = tmpNames[node->getNames().size() / 2].second;

    for (int blockNum : middleBlocks) {
        middle->addName(middleValue, blockNum);
    }

    middle->addChild(b1);
    middle->addChild(b2);

    std::cout << "path.size() : " << static_cast<int>(path.size()) << " " << static_cast<int>(path.size()) - 2 << std::endl;

    if (static_cast<int>(path.size()) - 2 >= 0) {
        for (size_t k = 0; k < path[path.size() - 2]->getChildrens().size(); k++) {
            if (path[path.size() - 2]->getChildrens()[k] == node) {
                path[path.size() - 2]->replaceChild(k, b1);
                path[path.size() - 2]->addChildAtPos(k + 1, b2);
                break;
            }
        }
    }
    else {
        root = middle;
    }

    return { middleValue, middleBlocks };
}

template <typename T>
void BtreeNode<T>::traverse(std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root, T data, int blockData) {
    if (path.empty()) {
        return;
    }

    path[path.size() - 1]->addName(data, blockData);

    if (path[path.size() - 1]->getNames().size() >= static_cast<size_t>(t)) {
        std::pair<T, std::vector<int>> newData = splitNode(path[path.size() - 1], path, root);
        path.pop_back();
        if (!path.empty()) {
            traverse(path, root, newData.first, newData.second[0]);
        }
    }
}

#endif

