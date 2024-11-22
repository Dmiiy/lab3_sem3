// include/data_structures/SetBinaryTree.h

#ifndef LAB3_SEM3_SETBINARYTREE_H
#define LAB3_SEM3_SETBINARYTREE_H

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include "BinaryTree.h"
#include "Common.h"
#include "ISet.h"


template <typename T>
class SetBinaryTree: public ISet<T> {
    BinaryTree<T> tree;  // For implementation, a binary search tree is used
public:
    // Constructors
    SetBinaryTree() = default;  // Empty set
    explicit SetBinaryTree(std::set<T> set) : tree(set) {}
    SetBinaryTree(const T *items, const int size) {
        for (int i = 0; i < size; i++)
            insert(items[i]);
    }
//    explicit SetBinaryTree(const char *str) {
//        std::istringstream in(str);
//        T value;
//        while (in >> value) tree.insert(value);
//    }
//    explicit SetBinaryTree(const std::string &str) {
//        std::istringstream in(str);
//        T value;
//        while (in >> value) tree.insert(value);
//    }

    // Size of the set
    int size() const override {
        return tree.getSize();
    }

    // Insert value into the set
    void insert(const T &value) override {
        if (tree.find(value)) return;
        tree.insert(value);
    }

    // Find value in the set
    bool find(const T &value) const override {
        return tree.find(value);
    }

    // Erase value from the set
    void erase(const T &value) override {
        tree.remove(value);
    }

    // Union of sets
    ISet<T>* setUnion(ISet<T> &s)  override{
        auto* result = new SetBinaryTree<T>();
        for (T x : tree) result->insert(x);
        for (T x : dynamic_cast<SetBinaryTree<T>&>(s)) result->insert(x);
        return result;
    }

    // Intersection of sets
    ISet<T>* intersection(ISet<T> &s) override {
        auto* result = new SetBinaryTree<T>();
        for (T x : tree) {
            if (s.find(x)) {
                result->insert(x);
            }
        }
        return result;
    }

    // Difference of sets
    ISet<T>* difference(ISet<T> &s) override{
        auto* result = new SetBinaryTree<T>();
        for (T x : tree) {
            if (!s.find(x)) {
                result->insert(x);
            }
        }
        return result;
    }

    // Is the current set a subset of another?
    bool subSet(const ISet<T> &set) const {
        const auto& otherSet = dynamic_cast<const SetBinaryTree<T>&>(set);
        for (T x : tree) {
            if (!otherSet.find(x)) return false;
        }
        return true;
    }

    // Are the sets equal?
    bool equal(const ISet<T> &set) const override {
        const auto& otherSet = dynamic_cast<const SetBinaryTree<T>&>(set);
        return this->subSet(otherSet) && otherSet.subSet(*this);
    }

    // Save to string
    std::string toString() const {
        // Сохраним элементы как вектор
        vector<T> elements;
        for (T x : tree) elements.push_back(x);
        // Отсортируем вектор по неубыванию
        sort(elements.begin(), elements.end());
        // Напечатаем отсортированные элементы в строку
        stringstream ss;
        for (T x : elements) ss << x << " ";
        return trim_copy(ss.str());
    }
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        typename BinaryTree<T>::Iterator iterator;
        explicit Iterator(typename BinaryTree<T>::Iterator iterator) : iterator(iterator) {}
        reference operator*() const {
            return *iterator;
        }
        pointer operator->() {
            return iterator;
        }
        Iterator& operator++() {
            ++iterator;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.iterator == b.iterator;
        }
        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.iterator != b.iterator;
        }
    };

    Iterator begin() const {
        return Iterator(tree.begin());
    }

    Iterator end() const {
        return Iterator(tree.end());
    }
};

#endif //LAB3_SEM3_SETBINARYTREE_H