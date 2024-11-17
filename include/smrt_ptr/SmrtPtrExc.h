

#ifndef LAB2_1_SMRTPTREXC_H
#define LAB2_1_SMRTPTREXC_H

#include <string>

using namespace std;

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif

class NullPointerException : public std::exception {
    std::string message_;

public:
    explicit NullPointerException(const std::string &message) : message_(message) {}

    explicit NullPointerException(const char *message) : message_(message) {}

    const char *what() const noexcept override {
        return message_.c_str();
    }
};

class AttemptToNullPointerFieldException : public std::exception {
    std::string message_;

public:
    explicit AttemptToNullPointerFieldException(const std::string &message) : message_(message) {}

    explicit AttemptToNullPointerFieldException(const char *message) : message_(message) {}

    const char *what() const noexcept override {
        return message_.c_str();
    }
};

#endif //LAB2_1_SMRTPTREXC_H
