#ifndef SHELLSORTER_GAP_H
#define SHELLSORTER_GAP_H

#include <functional>
#include "ISorter.h"

template <typename T>
class ShellSorterWithGap : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;
    ArraySequence<int> gaps;
public:
    ShellSorterWithGap(ArraySequence<int> gapSequence, std::function<bool(const T&, const T&)> comp)
            : gaps(gapSequence), comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        for (int i = 0; i < gaps.getLength(); i++) {
            int gap = gaps[i];
            // Вставляем элементы в подмассивы, определяемые разницей
            for (int j = gap; j < sequence->getLength(); j++) {
                T temp = (*sequence)[j];
                int k;

                // Сравниваем и перемещаем элементы, чтобы создать отсортированный подмассив
                for (k = j; k >= gap && comparator(temp, (*sequence)[k - gap]); k -= gap)
                    (*sequence)[k] = (*sequence)[k - gap];

                (*sequence)[k] = temp;
            }
        }
    }
};
#endif // SHELLSORTER_GAP_H
