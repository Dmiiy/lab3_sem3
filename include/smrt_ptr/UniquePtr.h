#ifndef LAB2_1_UNIQUEPTR_H
#define LAB2_1_UNIQUEPTR_H

#include "SmrtPtrExc.h"

template<typename T>
class UniquePtr {

private:
    T *m_ptr;

public:

    // constructors
    explicit UniquePtr(T *ptr = nullptr) : m_ptr(ptr) {}

    UniquePtr(const UniquePtr &) = delete;

    UniquePtr &operator=(const UniquePtr &) = delete;

    // move constructors
    UniquePtr(UniquePtr &&other) noexcept: m_ptr(other.m_ptr) {
        other.m_ptr = nullptr;
    }

    UniquePtr &operator=(UniquePtr &&other) {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // destructor
    ~UniquePtr() {
        delete m_ptr;
    }

    // methods
    T &operator*() {
        if (m_ptr != nullptr) {
            return *m_ptr;
        }
        throw NullPointerException("Null pointer exception");
    }

    const T &operator*() const {
        if (m_ptr != nullptr) {
            return *m_ptr;
        }
        throw NullPointerException("Null pointer exception");
    }

    T *operator->() {
        if (m_ptr != nullptr) {
            return m_ptr;
        }
        throw NullPointerException("Null pointer exception");
    }

    const T *operator->() const {
        if (m_ptr != nullptr) {
            return m_ptr;
        }
        throw NullPointerException("Null pointer exception");
    }

    T *release() {
        T *ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    void reset(T *ptr = nullptr) {
        delete m_ptr;
        m_ptr = ptr;
    }

    T *get() {
        return m_ptr;
    }

    const T *get() const {
        return m_ptr;
    }

    void swap(UniquePtr &other) {
        T *temp = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = temp;
    }

    void swap(T *&other) {
        T *temp = m_ptr;
        m_ptr = other;
        other = temp;
    }

    bool operator!=(T *ptr) {
        return m_ptr != ptr;
    }

    const bool operator!=(T *ptr) const {
        return m_ptr != ptr;
    }

};

#endif //LAB2_1_UNIQUEPTR_H
