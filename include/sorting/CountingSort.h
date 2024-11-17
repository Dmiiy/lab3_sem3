#ifndef COUNTINGSORTER_H
#define COUNTINGSORTER_H

#include <map>
#include <functional>
#include "ISorter.h"

template <typename T, typename Key>
class CountingSorter : public ISorter<T> {
private:
    std::function<bool(const Key&, const Key&)> comparator;
    std::function<Key(const T&)> keyExtractor;

public:
    CountingSorter(std::function<bool(const Key&, const Key&)> comp, std::function<Key(const T&)> extractor)
            : comparator(comp), keyExtractor(extractor) {}

    void sort(ArraySequence<T>* sequence) override {
        if (!sequence || sequence->getLength() == 0) {
            throw SortExc("Sequence is null");
        }

        std::map<Key, ArraySequence<T>, std::function<bool(const Key&, const Key&)>> countMap(comparator);

        // Подсчёт количества вхождений каждого элемента по ключу
        for (int i = 0; i < sequence->getLength(); ++i) {
            T item = (*sequence)[i];
            Key key = keyExtractor(item);
            countMap[key].append(item);
        }

        int index = 0;
        for (const auto& pair : countMap) {
            const ArraySequence<T>& itemsWithSameKey = pair.second;
            for (int j = 0; j < itemsWithSameKey.getLength(); ++j) {
                (*sequence)[index++] = itemsWithSameKey[j];
            }
        }
    }
};

#endif // COUNTINGSORTER_H
