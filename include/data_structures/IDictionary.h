
#ifndef LAB3_SEM3_IDICTIONARY_H
#define LAB3_SEM3_IDICTIONARY_H



#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility> // Для std::pair

#include "C:\Users\dimak\CLionProjects\lab3_sem3\include\sequence\ArraySequence.h"
// Чисто виртуальный класс IDictionary
template <typename TKey, typename TValue>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    // Получение элемента по ключу
    virtual TValue Get(const TKey& key) const = 0;

    // Проверка наличия ключа
    virtual bool ContainsKey(const TKey& key) const = 0;

    // Добавление элемента
    virtual void Add(const TKey& key, const TValue& value) = 0;

    // Удаление элемента
    virtual void Remove(const TKey& key) = 0;

    // Получение количества элементов
    virtual size_t GetCount() const = 0;

    // Получение всех ключей
    virtual ArraySequence<TKey> GetKeys() const = 0;

    // Получение всех значений
    virtual ArraySequence<TValue> GetValues() const = 0;
};

#endif //LAB3_SEM3_IDICTIONARY_H
