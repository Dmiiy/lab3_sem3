#ifndef MERGESORTER_H
#define MERGESORTER_H

#include <functional>
#include "ISorter.h"


template <typename T>
class MergeSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;
public:
    MergeSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        mergeSort(sequence, 0, sequence->getLength() - 1);
    }

private:
    void mergeSort(ArraySequence<T> *sequence, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(sequence, left, mid);
            mergeSort(sequence, mid + 1, right);
            merge(sequence, left, mid, right);
        }
    }

    void merge(ArraySequence<T> *sequence, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        ArraySequence<T> L;
        ArraySequence<T> R;

        for (int i = 0; i < n1; ++i)
            L.append((*sequence)[left + i]);
        for (int i = 0; i < n2; ++i)
            R.append((*sequence)[mid + 1 + i]);

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (comparator(L[i], R[j])) {
                (*sequence)[k] = L[i];
                ++i;
            } else {
                (*sequence)[k] = R[j];
                ++j;
            }
            ++k;
        }
        while (i < n1) {
            (*sequence)[k] = L[i];
            ++i;
            ++k;
        }
        while (j < n2) {
            (*sequence)[k] = R[j];
            ++j;
            ++k;
        }
    }
};

#endif // MERGESORTER_H
