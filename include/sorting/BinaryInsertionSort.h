#ifndef BINARYINSERTIONSORTER_H
#define BINARYINSERTIONSORTER_H

#include <functional>
#include "ISorter.h"


template <typename T>
class BinaryInsertionSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;
public:
    BinaryInsertionSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override{
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();
        for (int i = 1; i < n; ++i) {
            T key = (*sequence)[i];
            int insertedPos = binarySearch(sequence, key, 0, i - 1);

            // Сдвиг элементов
            for (int j = i - 1; j >= insertedPos; --j) {
                (*sequence)[j + 1] = (*sequence)[j];
            }
            (*sequence)[insertedPos] = key;
        }
    }

private:
    int binarySearch(ArraySequence<T> *sequence, T key, int low, int high) {
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (comparator(key, (*sequence)[mid])) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return low;
    }
};
#endif // BINARYINSERTIONSORTER_H
