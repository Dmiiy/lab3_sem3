//
// Created by dimak on 04.11.2024.
//

#ifndef LAB2_SEM3_SORTEXC_H
#define LAB2_SEM3_SORTEXC_H
#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#include <string>

using namespace std;

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif

//Исключения

class SortExc : public std::exception {
    std::string message_;

public:
    explicit SortExc(const std::string &message) : message_(message) {}

    explicit SortExc(const char *message) : message_(message) {}

    const char *what() const noexcept override {
        return message_.c_str();
    }
};

#endif

#endif //LAB2_SEM3_SORTEXC_H
