#ifndef ARRAYSEQUENCE_INCLUDED
#define ARRAYSEQUENCE_INCLUDED

#include "DynamicArray.h"
#include "Sequence.h"

template<class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> data;

public:
    // Создание объекта

    ArraySequence(T *items, int count) : data(items, count) {}

    ArraySequence() : data() {}

    explicit ArraySequence(const DynamicArray<T> &array) : data(array) {}

    // Декомпозиция
    T getFirst() const override {
        return data.get(0);
    }

    T getLast() const override {
        return data.get(data.getSize() - 1);
    }

    T get(int index) const override {
        return data.get(index);
    }

    // Перегруженные операторы
    T operator[](int i) const override {
        return data[i];
    }

    T &operator[](int i) override {
        return data[i];
    }

    Sequence<T> *getSubsequence(int startIndex, int endIndex) const override {
        if (startIndex > endIndex) {
            throw "Index startIndex <= endIndex";
        }
        int size = endIndex - startIndex + 1;
        DynamicArray<T> da(size);
        for (int i = 0; i < size; ++i) {
            da[i] = get(startIndex + i);
        }
        return new ArraySequence<T>(da);
    }

    int getLength() const override {
        return data.getSize();
    }

    // Операции
    void append(T item) override {
        int size = data.getSize();
        data.resize(size + 1);
        data.set(size, item);
    }

    void prepend(T item) override {
        int size = data.getSize();
        data.resize(size + 1);
        data.define_resize(size + 1);
        for (int i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = item;
        data.define_set(0, true);
    }

    void insertAt(T item, int index) override {
        int size = data.getSize();
        data.resize(size + 1);
        data.define_resize(size + 1);
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data.set(index, item);
    }

    Sequence<T> *concat(Sequence<T> *list) override {
        auto *result = new ArraySequence<T>(this->data);
        result->data.resize(getLength() + list->getLength());
        for (int i = 0; i < list->getLength(); ++i) {
            result->data.set(getLength() + i, list->get(i));
        }
        return result;
    }

    void removeAt(int index) override {
        int size = data.getSize();
        for (int i = index + 1; i < size; ++i) {
            data[i - 1] = data[i];
        }
        data.define_resize(size - 1);
        data.resize(size - 1);
    }
    void clear() override {
        data.clear();
    }

    // Виртуальный деструктор
    virtual ~ArraySequence<T>() {}

    Sequence<T> *map(T (*f)(T)) const override {
        Sequence<T> *res = new ArraySequence<T>();
        for (int i = 0; i < getLength(); ++i) {
            res->append(f(data.get(i)));
        }
        return res;
    }

    Sequence<T> *where(bool (*h)(T)) const override {
        auto *res = new ArraySequence<T>;
        for (int i = 0; i < getLength(); ++i) {
            T item = data.get(i);
            if (h(item)) {
                res->append(item);
            }
        }
        return res;
    }

    T reduce(T (*f)(T, T)) const override {
        T result = data.get(0);
        for (int i = 1; i < data.getSize(); ++i) {
            result = f(result, data.get(i));
        }
        return result;
    }
};

#endif
