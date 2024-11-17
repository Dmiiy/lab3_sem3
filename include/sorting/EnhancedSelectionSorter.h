#ifndef ENHANCED_SELECTION_SORTER_H
#define ENHANCED_SELECTION_SORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class EnhancedSelectionSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    EnhancedSelectionSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();
        for (int i = 0; i < n - 1; ++i) {
            int minIndex = i;
            for (int j = i + 1; j < n; ++j) {
                if (!comparator((*sequence)[minIndex], (*sequence)[j])) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                std::swap((*sequence)[i], (*sequence)[minIndex]);
            }
        }
    }
};

#endif // ENHANCED_SELECTION_SORTER_H
