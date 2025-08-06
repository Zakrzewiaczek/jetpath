#ifndef BTREE_CPP  
#define BTREE_CPP 

#include "bTree.h"

template<typename T>
bool isGreater(const T& a, const T& b) {
    return a > b;
}

template<typename T>
bool isEqual(const T& a, const T& b) {
    return a == b;
}

template <typename T>
Btree<T>::Btree(int t) {
    this->t = t;
    root = new BtreeNode<T>(t);
}

template <typename T>
void Btree<T>::insert(const T& data, int blocNum) {
    std::vector<BtreeNode<T>*> path;
    BtreeNode<T>* tmp = root;

    while (!tmp->getChildrens().empty()) {
        bool moved = false;
        bool finded = false;
        for (size_t i = 0; i < tmp->getNames().size(); ++i) {
            if (isEqual(data, tmp->getNames()[i].first)) {
                //path.push_back(tmp);
                finded = true;
                break;
            }
            if (!isGreater(data, tmp->getNames()[i].first)) {
                path.push_back(tmp);
                tmp = tmp->getChildrens()[i];
                moved = true;
                break;
            }
        }
        if (finded) {
            break;
        }
        if (!moved) {
            path.push_back(tmp);
            tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
        }
    }

    path.push_back(tmp);
    tmp->insert(data, blocNum, path, root);
}

template <typename T>
std::vector<std::pair<T, std::vector<int>>> Btree<T>::getRootNames() {
    return root->getNames();
}

template <typename T>
BtreeNode<T>* Btree<T>::getRoot() {
    return root;
}

template <typename T>
std::vector<int> Btree<T>::getBlockNum(T val) {
    std::vector<int> blockNum;
    BtreeNode<T>* tmp = root;
    bool goOut = false;
    while (!tmp->getChildrens().empty()) {
        bool moved = false;
        goOut = false;
        // Sprawdź klucze w bieżącym węźle
        for (size_t i = 0; i < tmp->getNames().size(); i++) {
            // Sprawdź czy klucz jest równy szukanej wartości
            if (isEqual(val, tmp->getNames()[i].first)) {
                // Dodaj wszystkie numery bloków dla tego klucza
                for (int j = 0; j < tmp->getNames()[i].second.size(); j++) {
                    blockNum.push_back(tmp->getNames()[i].second[j]);
                }
                goOut = true;
                break;
                /*
                for (const auto& block : tmp->getNames()[i].second) {
                    blockNum.push_back(block);
                }
                */
            }

            // Przejdź do odpowiedniego dziecka, jeśli wartość jest mniejsza
            if (!isGreater(val, tmp->getNames()[i].first) && !isEqual(val, tmp->getNames()[i].first)) {
                tmp = tmp->getChildrens()[i];
                moved = true;
                break;
            }
        }
        if (goOut) {
            break;
        }
        // Jeśli nie przesunięto się do żadnego dziecka, idź do ostatniego dziecka
        if (!moved) {
            tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
        }
    }

    // Sprawdź klucze w liściu
    if (!goOut) {
        for (size_t i = 0; i < tmp->getNames().size(); i++) {
            if (isEqual(val, tmp->getNames()[i].first)) {
                for (int j = 0; j < tmp->getNames()[i].second.size(); j++) {
                    blockNum.push_back(tmp->getNames()[i].second[j]);
                }
            }
        }
    }
    return blockNum;
}

template <typename T>
void Btree<T>::printTree(BtreeNode<T>* node, const std::string& prefix, bool isLast) {
    if (node == nullptr) return;

    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");

    std::cout << "[";
    for (const auto& name : node->getNames()) {
        std::cout << " " << name.first;
    }
    std::cout << " ]" << std::endl;

    auto children = node->getChildrens();
    for (size_t i = 0; i < children.size(); ++i) {
        std::string newPrefix = prefix + (isLast ? "    " : "│   ");
        bool lastChild = (i == children.size() - 1);
        printTree(children[i], newPrefix, lastChild);
    }
}

// Zakomentowany kod z pierwotnego pliku - pozostawiony dla kompletności
/*
template <typename T>
std::vector<int> Btree<T>::getBlockNumByOperator(T val, std::string op) {
    std::vector<int> blockNum;

    auto compareByOperator = [&](const T& nodeVal) -> bool {
        if (op == "<") return !isGreater(val, nodeVal) && !isEqual(val, nodeVal);
        if (op == "<=") return !isGreater(val, nodeVal);
        if (op == ">") return isGreater(nodeVal, val);
        if (op == ">=") return isGreater(nodeVal, val) || isEqual(val, nodeVal);
        if (op == "!=") return !isEqual(val, nodeVal);
        return false;
    };

    std::function<void(BtreeNode<T>*)> search = [&](BtreeNode<T>* node) {
        if (!node) return;

        for (size_t i = 0; i < node->getNames().size(); i++) {
            if (compareByOperator(node->getNames()[i].first)) {
                for (const auto& blockId : node->getNames()[i].second) {
                    blockNum.push_back(blockId);
                }
            }
        }

        if (op == "<" || op == "<=" || op == "!=" || op == ">" || op == ">=") {
            for (auto& child : node->getChildrens()) {
                search(child);
            }
        }
    };

    search(root);
    return blockNum;
}
*/

#endif
