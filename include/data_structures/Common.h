
#ifndef LAB3_SEM3_COMMON_H
#define LAB3_SEM3_COMMON_H

#include <algorithm>
#include <string>


// == Исключения ==

// Выход индекса за допустимые границы: отрицательный, >= размера массива
// Индекс должен быть в пределах: 0..количество_элементов-1
class IndexOutOfRange : public std::exception {
    std::string message_;

public:
    explicit IndexOutOfRange(const std::string &message) : message_(message) {}
    explicit IndexOutOfRange(const char *message) : message_(message) {}
    const char *what() const noexcept override {
        return message_.c_str();
    }
};

#endif
