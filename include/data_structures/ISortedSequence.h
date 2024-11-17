//
// Created by dimak on 15.11.2024.
//

#ifndef LAB3_SEM3_ISORTEDSEQUENCE_H
#define LAB3_SEM3_ISORTEDSEQUENCE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility>
template <typename TElement>
class ISortedSequence {
public:
    virtual ~ISortedSequence() = default;

    // Получение длины последовательности
    virtual size_t GetLength() const = 0;

    // Проверка на пустоту
    virtual bool IsEmpty() const = 0;

    // Получение элемента по индексу
    virtual TElement Get(int index) const = 0;

    // Добавление элемента с автоматической сортировкой
    virtual void Add(const TElement& element) = 0;

    // Получение первого элемента
    virtual TElement GetFirst() const = 0;

    // Получение последнего элемента
    virtual TElement GetLast() const = 0;

    // Получение индекса элемента (или -1, если не найден)
    virtual int IndexOf(const TElement& element) const = 0;

    // Получение подпоследовательности
    virtual ISortedSequence<TElement>* GetSubsequence(int startIndex, int endIndex) const = 0;
};

#endif //LAB3_SEM3_ISORTEDSEQUENCE_H
