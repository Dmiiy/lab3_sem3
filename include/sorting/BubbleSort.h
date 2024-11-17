#ifndef BUBBLESORTER_H
#define BUBBLESORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class BubbleSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    BubbleSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (!comparator((*sequence)[j], (*sequence)[j + 1])) {
                    std::swap((*sequence)[j], (*sequence)[j + 1]);
                }
            }
        }
    }
};

#endif // BUBBLESORTER_H
