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
#include "../sequence/ArraySequence.h"


template <typename T>
class SetBinaryTree: public ISet<T> {
private:
    BinaryTree<T> tree;
public:
    // Constructors
    SetBinaryTree() = default;  // Empty set
    explicit SetBinaryTree(const ArraySequence<T>& sequence) {
        for (int i = 0; i < sequence.getLength(); i++)
            insert(sequence.get(i));
    }
    SetBinaryTree(const SetBinaryTree<T>& set) : tree(set.tree) {}
    explicit SetBinaryTree(std::set<T> set) : tree(set) {}

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
    bool subSet(const ISet<T> &set) const override {
        if (this->size() > set.size()) return false; // Оптимизация по размеру
        for (const T& x : tree) {
            if (!set.find(x)) return false;
        }
        return true;
    }

    // Are the sets equal?
    bool equal(const ISet<T> &set) const override {
        return this->size() == set.size() && this->subSet(set);
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