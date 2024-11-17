#ifndef HEAPSORTER_H
#define HEAPSORTER_H

#include <functional>
#include "ISorter.h"

template <typename T>
class HeapSorter : public ISorter<T> {
private:
    std::function<bool(const T&, const T&)> comparator;
public:
    HeapSorter(std::function<bool(const T&, const T&)> comp)
            : comparator(comp) {}

    void sort(ArraySequence<T> *sequence) override {
        if (sequence==nullptr) {
            throw SortExc("Sequence is null");
        }
        int n = sequence->getLength();

        // Построение кучи (перегруппировка массива)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(sequence, n, i);

        // Один за другим извлекаем элементы из кучи
        for (int i = n - 1; i > 0; i--) {
            std::swap((*sequence)[0], (*sequence)[i]); // Перемещение текущего корня в конец
            heapify(sequence, i, 0); // Вызываем heapify на уменьшенной куче
        }
    }

private:
    void heapify(ArraySequence<T> *sequence, int n, int i) {
        int largest = i; // Инициализация самого большого элемента как корня
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // Если левый дочерний элемент больше корня
        if (left < n && comparator((*sequence)[largest], (*sequence)[left]))
            largest = left;

        // Если правый дочерний элемент больше самого большого элемента
        if (right < n && comparator((*sequence)[largest], (*sequence)[right]))
            largest = right;

        // Если самый большой элемент не корень
        if (largest != i) {
            std::swap((*sequence)[i], (*sequence)[largest]); // Обмен

            // Рекурсивно преобразуем в куче затронутое поддерево
            heapify(sequence, n, largest);
        }
    }
};

#endif // HEAPSORTER_H
