#ifndef DYNAMICARRAY_INCLUDED
#define DYNAMICARRAY_INCLUDED

#include <iostream>

template<class T>
class DynamicArray {
    int size;
    int capacity;  // Текущая емкость массива
    T *data;
    bool *defined;  // Задан ли элемент массива?

    void checkIndex(int index) const {
        if (index < 0 || index >= size) {
            std::cerr << "Index " << index << " out of range 0.." << (size - 1) << "\n";
            throw "Index out of range";
        }
    }

    // Увеличивает емкость массива в два раза, если необходимо
    void ensureCapacity(int minCapacity) {
        if (minCapacity > capacity) {
            int newCapacity = (minCapacity > capacity * 2) ? minCapacity : capacity * 2;
            T *newData = new T[newCapacity];
            bool *newDefined = new bool[newCapacity];

            // Копируем старые данные в новый массив
            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
                newDefined[i] = defined[i];
            }

            // Заполняем неиспользуемую часть нового массива
            for (int i = size; i < newCapacity; ++i) {
                newDefined[i] = false;
            }

            delete[] data;
            delete[] defined;

            data = newData;
            defined = newDefined;
            capacity = newCapacity;
        }
    }

public:
    // Создание объекта
    DynamicArray(T *items, int count) : size(count), capacity(count) {
        if (size < 0) throw "Size < 0";
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = items[i];
            defined[i] = true;
        }
    }

    explicit DynamicArray(int count = 0) : size(count), capacity(count > 0 ? count : 1) {
        if (size < 0) throw "Count < 0";
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            defined[i] = false;
        }
    }

    DynamicArray(const DynamicArray<T> &dynamicArray) {
        size = dynamicArray.size;
        capacity = dynamicArray.capacity;
        data = new T[capacity];
        defined = new bool[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
            defined[i] = dynamicArray.defined[i];
        }
    }

    // Деструктор
    ~DynamicArray() {
        delete[] data;
        delete[] defined;
    }
    void clear() {
        delete[] data;
        delete[] defined;

        // Сброс массива до начального состояния
        size = 0;
        capacity = 1;
        data = new T[capacity];
        defined = new bool[capacity]{false};
    }


    // Декомпозиция
    T &get(int index) {
        checkIndex(index);
        if (!defined[index]) {
            throw "Element with index not defined";
        }
        return data[index];
    }

    const T &get(int index) const {
        checkIndex(index);
        if (!defined[index]) {
            throw "Element with index not defined";
        }
        return data[index];
    }

    int getSize() const {
        return size;
    }

    // Операции
    void set(int index, T value) {
        checkIndex(index);
        data[index] = value;
        defined[index] = true;
    }

    T operator[](int index) const {
        return get(index);
    }

    T &operator[](int index) {
        checkIndex(index);
        defined[index] = true;
        return data[index];
    }

    void resize(int newSize) {
        if (newSize < 0) throw "Negative size error";
        ensureCapacity(newSize);  // проверка и увеличение емкости при необходимости
        size = newSize;
    }

    void define_resize(int newSize) {
        if (newSize < 0) throw "Negative size error";
        for (int i = newSize - 1; i >= 1; --i) {
            defined[i] = defined[i - 1];
        }
    }

    void define_set(int index, bool value) {
        if (index >= 0 && index < size) {
            defined[index] = value;
        }
    }
};

#endif
