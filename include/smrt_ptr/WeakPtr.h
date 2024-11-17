#ifndef WEAK_PTR_H
#define WEAK_PTR_H

#include "SharedPtr.h"
#include "ControlBlock.h"

template<class T>
class WeakPtr {
private:
    ControlBlock<T> *cb = nullptr;

public:

    //constructors
    WeakPtr() = default;

    WeakPtr(const SharedPtr<T> &sp) : cb(sp.cb) {
        if (cb != nullptr) {
            cb->weak_plus();
        }
    }

    WeakPtr(const WeakPtr &wp) : cb(wp.cb) {
        if (cb != nullptr) {
            cb->weak_plus();
        }
    }

    WeakPtr &operator=(const WeakPtr &wp) {
        if (this != &wp) {
            reset();
            cb = wp.cb;
            if (cb != nullptr) {
                cb->weak_plus();
            }
        }
        return *this;
    }

    //move constructors
    WeakPtr(WeakPtr &&other) noexcept: cb(other.cb) {
        other.cb = nullptr;
    }

    WeakPtr &operator=(WeakPtr &&other) noexcept {
        if (this != &other) {
            reset();
            cb = other.cb;
            other.cb = nullptr;
        }
        return *this;
    }

    //destructors
    ~WeakPtr() {
        reset();
    }

    //methods
    SharedPtr<T> lock(){
        return expired() ? SharedPtr<T>(nullptr) : SharedPtr<T>(cb, true);
    }

    bool expired() const {
        return cb == nullptr || cb->get() == 0;
    }

    void reset() {
        if (cb == nullptr) {
            return;
        }

        cb->weak_minus();

        if (cb->get_weak_count() == 0 && cb->get() == 0) {
            delete cb;
        }

        cb = nullptr;
    }


    void swap(WeakPtr &other) {
        ControlBlock<T> *temp = cb;
        cb = other.cb;
        other.cb = temp;
    }

    unsigned int use_count() const {
        return cb ? cb->get() : 0;
    }

    unsigned int weak_count() const {
        return cb ? cb->get_weak_count() : 0;
    }
};

#endif // WEAK_PTR_H
