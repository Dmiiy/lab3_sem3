#ifndef BATCHERSORTER_H
#define BATCHERSORTER_H

#include <functional>
#include "ISorter.h"


template <typename T>
class BatcherSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;
    //static HelpClass helpClass;

public:
    BatcherSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        mergeSort(sequence, 0, sequence->getLength());
    }

private:
    void mergeSort(ArraySequence<T> *sequence, int left, int right) {
        if (right - left < 2) return; // Базовый случай

        int mid = (left + right) / 2;

        mergeSort(sequence, left, mid);
        mergeSort(sequence, mid, right);
        merge(sequence, left, mid, right);
    }

    void merge(ArraySequence<T> *sequence, int left, int mid, int right) {
        int leftSize = mid - left;
        int rightSize = right - mid;

        ArraySequence<T> leftArray;
        ArraySequence<T> rightArray;

        for (int i = 0; i < leftSize; ++i)
            leftArray.append(sequence->get(left + i));
        for (int i = 0; i < rightSize; ++i)
            rightArray.append(sequence->get(mid + i));

        // Слияние
        int i = 0, j = 0, k = left;
        while (i < leftSize && j < rightSize) {
            if (comparator(leftArray[i], rightArray[j])) {
                (*sequence)[k++] = leftArray[i++];
            } else {
                (*sequence)[k++] = rightArray[j++];
            }
        }

        while (i < leftSize) {
            (*sequence)[k++] = leftArray[i++];
        }

        while (j < rightSize) {
            (*sequence)[k++] = rightArray[j++];
        }
    }
};
//template <typename T>
//HelpClass BatcherSorter<T>::helpClass;
#endif // BATCHERSORTER_H
