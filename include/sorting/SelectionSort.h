#ifndef SELECTIONSORTER_H
#define SELECTIONSORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class SelectionSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    SelectionSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();
        for (int i = 0; i < n - 1; ++i) {
            int minIndex = i;
            for (int j = i + 1; j < n; ++j) {
                if (comparator((*sequence)[j], (*sequence)[minIndex])) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                std::swap((*sequence)[i], (*sequence)[minIndex]);
            }
        }
    }
};

#endif // SELECTIONSORTER_H
