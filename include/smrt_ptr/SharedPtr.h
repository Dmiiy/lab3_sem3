#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include "ControlBlock.h"
#include "SmrtPtrExc.h"

template<typename T>
class SharedPtr {
    template<typename U>
    friend class WeakPtr;

private:
    ControlBlock<T> *cb;

    SharedPtr(ControlBlock<T> *cb, bool copy) : cb(cb) {
        if (cb->get_ptr() != nullptr) {
            cb->ref_plus();
        }
    }
public:

    //Constructors

    SharedPtr(T *ptr = nullptr) noexcept {
        cb = new ControlBlock<T>(ptr);
        if (cb->get_ptr() != nullptr) {
            cb->ref_plus();
        }
    }

    SharedPtr(const SharedPtr<T> &other) : cb(other.cb) {
        if (cb->get_ptr() != nullptr) {
            cb->ref_plus();
        }
    }

    SharedPtr &operator=(const SharedPtr &other) {
        if (this != &other) {
            delete_ptr();
            cb = other.cb;
            if (cb->get_ptr() != nullptr) {
                cb->ref_plus();
            }
        }
        return *this;
    }

    //move constructors
    SharedPtr(SharedPtr &&other) noexcept: cb(other.cb) {
        other.cb = nullptr;
    }

    SharedPtr &operator=(SharedPtr &&other) noexcept {
        if (this != &other) {
            delete_ptr();
            cb = other.cb;
            other.cb = nullptr;
        }
        return *this;
    }

    //destructor
    ~SharedPtr() {
        delete_ptr();
    }

    //methods
    void delete_ptr() {

        if (!cb) {
            return;
        }

        cb->ref_minus();

        if (cb->get() != 0) {
            return;
        }

        cb->delete_ptr();

        if (cb->get_weak_count() == 0) {
            delete cb;
        }

    }

    ControlBlock<T> *get_cb() const {
        return cb;
    }

    void reset(T *ptr = nullptr) {
        delete_ptr();
        cb = new ControlBlock<T>(ptr);
        if (cb->get_ptr() != nullptr) {
            cb->ref_plus();
        }
    }

    const T *get() const {
        return cb ? cb->get_ptr() : nullptr;
    }

    T *get() {
        return cb ? cb->get_ptr() : nullptr;
    }

    const T &operator*() const {
        if (cb == nullptr || cb->get_ptr() == nullptr) {
            throw NullPointerException("Dereferencing null pointer");
        }
        return *cb->get_ptr();
    }

    T &operator*() {
        if (cb == nullptr || cb->get_ptr() == nullptr) {
            throw NullPointerException("Dereferencing null pointer");
        }
        return *cb->get_ptr();
    }

    const T *operator->() const {
        if (cb == nullptr || cb->get_ptr() == nullptr) {
            throw AttemptToNullPointerFieldException("Accessing field through null pointer");
        }
        return cb->get_ptr();
    }

    T *operator->() {
        if (cb == nullptr || cb->get_ptr() == nullptr) {
            throw AttemptToNullPointerFieldException("Accessing field through null pointer");
        }
        return cb->get_ptr();
    }

    unsigned int use_count() const {
        return cb ? cb->get() : 0;
    }

    void swap(SharedPtr &other) {
        ControlBlock<T> *temp = cb;
        cb = other.cb;
        other.cb = temp;
    }

};

#endif // SHARED_PTR_H
