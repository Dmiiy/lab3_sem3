#ifndef SHELLSORTER_H
#define SHELLSORTER_H

#include <functional>
#include "ISorter.h"


template <typename T>
class ShellSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;

public:
    ShellSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();

        // Начинаем с большого шага и уменьшаем его
        for (int gap = n / 2; gap > 0; gap /= 2) {
            // Вставляем элементы в подмассивы, определяемые разницей
            for (int i = gap; i < n; i++) {
                T temp = (*sequence)[i];
                int j;

                // Сравниваем и перемещаем элементы, чтобы создать отсортированный подмассив
                for (j = i; j >= gap && comparator(temp, (*sequence)[j - gap]); j -= gap)
                    (*sequence)[j] = (*sequence)[j - gap];

                (*sequence)[j] = temp;
            }
        }
    }
};
#endif // SHELLSORTER_H
