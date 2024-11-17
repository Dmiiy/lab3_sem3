#ifndef INSERTIONSORTER_H
#define INSERTIONSORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class InsertionSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    InsertionSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();
        for (int i = 1; i < n; ++i) {
            T key = (*sequence)[i];
            int j = i - 1;

            while (j >= 0 && comparator(key, (*sequence)[j])) {
                (*sequence)[j + 1] = (*sequence)[j];
                --j;
            }
            (*sequence)[j + 1] = key;
        }
    }
};

#endif // INSERTIONSORTER_H
