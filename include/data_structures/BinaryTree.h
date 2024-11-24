#ifndef LAB3_SEM3_BINARYTREE_H
#define LAB3_SEM3_BINARYTREE_H
#include <algorithm>
#include <cassert>
#include <codecvt>
#include <cstring>
#include <iostream>
#include <locale>
#include <set>
#include <sstream>

#include "Common.h"
#include "../sequence/ArraySequence.h"

using namespace std;

template <typename T>
class BinaryTree {
private:
    // Forward declarations
    struct Node;
    struct Operation {
        virtual void apply(Node *n) = 0;
    };

    // Node structure definition
    struct Node {
        T value;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *next = nullptr;
        int height = 1;
        int dis = 0;

        void reCalc() {
            int leftHeight = (left!= nullptr) ? left->height : 0;
            int rightHeight = (right!= nullptr) ? right->height : 0;
            height = std::max(leftHeight, rightHeight) + 1;
            dis = leftHeight - rightHeight;
        }

        explicit Node(T value, Node *left = nullptr, Node *right = nullptr)
                : value(value), left(left), right(right) {
            reCalc();
        }

        void NLR(Operation &op) {
            op.apply(this);
            if (left) left->NLR(op);
            if (right) right->NLR(op);
        }

        void NRL(Operation &op) {
            op.apply(this);
            if (right) right->NRL(op);
            if (left) left->NRL(op);
        }

        void LRN(Operation &op) {
            if (left) left->LRN(op);
            if (right) right->LRN(op);
            op.apply(this);
        }

        void LNR(Operation &op) {
            if (left) left->LNR(op);
            op.apply(this);
            if (right) right->LNR(op);
        }

        void RLN(Operation &op) {
            if (right) right->RLN(op);
            if (left) left->RLN(op);
            op.apply(this);
        }

        void RNL(Operation &op) {
            if (right) right->RNL(op);
            op.apply(this);
            if (left) left->RNL(op);
        }
    };

    Node *root = nullptr;
    int size = 0;
    Node *first;  // For threading

    // Private utility methods
    void delTree(Node *tree) {
        if (tree == nullptr) return;
        delTree(tree->left);
        delTree(tree->right);
        delete tree;
    }

    void NLR(Node *n, Operation &op) {
        if (n == nullptr) return;
        op.apply(n);
        NLR(n->left, op);
        NLR(n->right, op);
    }
    void LNR(Node *n, Operation &op) {
        if (n == nullptr) return;
        op.apply(n);
        LNR(n->left, op);
        LNR(n->right, op);
    }

    void go(Node *n, const char *order, Operation &op) {
        if (n == nullptr) return;
        for (size_t i = 0, len = strlen(order); i < len; i++) {
            switch (order[i]) {
                case 'L': go(n->left, order, op); break;
                case 'R': go(n->right, order, op); break;
                case 'N': op.apply(n); break;
            }
        }
    }

    Node *copy(Node *n) {
        if (n == nullptr) return nullptr;
        return new Node(n->value, copy(n->left), copy(n->right));
    }

    Node *insertTo(Node *n, const T &v) {
        if (v <= n->value) {
            if (n->left)
                n->left = insertTo(n->left, v);
            else
                n->left = new Node(v);
        } else {
            if (n->right)
                n->right = insertTo(n->right, v);
            else
                n->right = new Node(v);
        }
        return balance(n);
    }

public:
    // Constructors and destructor
    BinaryTree() = default;

    explicit BinaryTree(const ArraySequence<T> &seq) {
        for (int i = 0; i < seq.getLength(); i++)
            insert(seq.get(i));
    }
    explicit BinaryTree(const std::set<T> &set) {
        for (T x : set)
            insert(x);
    }

    ~BinaryTree() {
        delTree(root);
    }

    // Basic operations
    int getSize() const { return size; }
    Node *getRoot() { return root; }

    void insert(const T &value) {
        size++;
        if (root)
            root = insertTo(root, value);
        else
            root = new Node(value);
    }

    Node *find(const T &v) const {
        Node *n = root;
        while (n != nullptr) {
            if (v == n->value) return n;
            if (v < n->value)
                n = n->left;
            else
                n = n->right;
        }
        return nullptr;
    }

    void remove(const T &v) {
        root = remove(root, v);
    }

    Node *remove(Node *r, const T &v) {
        if (r == nullptr) return nullptr;

        if (v < r->value) {
            r->left = remove(r->left, v);
            return balance(r);
        }
        if (v > r->value) {
            r->right = remove(r->right, v);
            return balance(r);
        }

        assert(v == r->value);
        if (r->left && r->right) {
            r->value = minimum(r->right)->value;
            r->right = remove(r->right, r->value);
        } else {
            Node *toDelete = r;
            if (r->left)
                r = r->left;
            else if (r->right)
                r = r->right;
            else
                r = nullptr;
            size--;
            delete toDelete;
        }
        return balance(r);
    }

    // Tree properties
    int height() const { return root ? root->height : 0; }

    int height(Node *n) {
        if (!n) return 0;
        return max(height(n->left), height(n->right)) + 1;
    }

    Node *minimum(Node *n) const {
        if (n == nullptr) throw range_error("Empty tree");
        while (n->left) n = n->left;
        return n;
    }

    Node *maximum(Node *n) const {
        if (n == nullptr) throw range_error("Empty tree");
        while (n->right) n = n->right;
        return n;
    }

    T getMin()const {
        return minimum(root)->value;
    }
    T getMax()const {
        return maximum(root)->value;
    }

    // Balance operations
    Node *balance(Node *a) {
        if (a == nullptr) return nullptr;
        a->reCalc();

        if (a->dis <= -2) {
            if (a->right->dis <= 0)
                a = leftRotation(a);
            else
                a = bigLeftRotation(a);
        } else if (a->dis >= 2) {
            if (a->left->dis >= 0)
                a = rightRotation(a);
            else
                a = bigRightRotation(a);
        }
        return a;
    }

    Node *leftRotation(Node *a) {
        Node *b = a->right;
        a->right = b->left;
        a->reCalc();
        b->left = a;
        b->reCalc();
        return b;
    }

    Node *rightRotation(Node *a) {
        assert(a->left != nullptr);
        Node *b = a->left;
        a->left = b->right;
        a->reCalc();
        b->right = a;
        b->reCalc();
        return b;
    }

    Node *bigLeftRotation(Node *a) {
        Node *b = a->right;
        a->right = rightRotation(b);
        return leftRotation(a);
    }

    Node *bigRightRotation(Node *a) {
        Node *newRoot = a->left;
        a->left = leftRotation(newRoot);
        return rightRotation(a);
    }

    // Threading operations
    struct Thread : public Operation {
        Node *first = nullptr;
        Node *last = nullptr;
        void apply(Node *n) override {
            if (first == nullptr) first = n;
            if (last != nullptr) last->next = n;
            n->next = nullptr;
            last = n;
        }
    };

    Node *thread() {
        first = nullptr;
        Thread td;
        NLR(root, td);
        first = td.first;
        return first;
    }

    Node *thread(const char *order) {
        assert(strlen(order) == 3);
        first = nullptr;
        Thread td;
        go(root, order, td);
        first = td.first;
        return first;
    }
    // Итератор для BinaryTree
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        // Создаём обход и запоминаем индекс
        explicit Iterator(Node *node, int index) : index(index), root(node) {
            LNR(node);
        }
        reference operator*() const {
            return path[index]->value;
        }
        pointer operator->() {
            return path[index]->value;
        }
        Iterator &operator++() {
            index++;
            if (index > path.size()) index = path.size();
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.index == b.index && a.root == b.root;
        };
        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.index != b.index || a.root != b.root;
        };

    private:
        void LNR(Node *node) {
            if (node == nullptr) return;
            if (node->left != nullptr) LNR(node->left);
            path.push_back(node);  // Добавляем узел в путь
            if (node->right != nullptr) LNR(node->right);
        }
        std::vector<Node *> path;
        int index;
        Node *root;
    };
    Iterator begin() const {
        return Iterator(root, 0);
    }
    Iterator end() const {
        return Iterator(root, getSize());
    }
};

#endif //LAB3_SEM3_BINARYTREE_H