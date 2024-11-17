#ifndef LAB2_1_CONTROLBLOCK_H
#define LAB2_1_CONTROLBLOCK_H

template<typename T>
class ControlBlock {
private:
    unsigned int ref_count{};
    unsigned int weak_count{};
    T *ptr = nullptr;
public:
    ControlBlock(T *m_ptr = nullptr) : ref_count(0), weak_count(0), ptr(m_ptr) {}

    ControlBlock(const ControlBlock &) = delete;

    ControlBlock &operator=(const ControlBlock &) = delete;

    ~ControlBlock() {};

    T *get_ptr() {
        return ptr;
    }

    void delete_ptr() {
        delete ptr;
    }

    unsigned int get() {
        return ref_count;
    }

    void ref_plus()
    {
        ref_count++;
    }

    void ref_minus()
    {
        ref_count--;
    }

    unsigned int get_weak_count() {
        return weak_count;
    }

    void weak_minus() {
        weak_count--;
    }

    void weak_plus() {
        weak_count++;
    }

};


#endif //LAB2_1_CONTROLBLOCK_H
