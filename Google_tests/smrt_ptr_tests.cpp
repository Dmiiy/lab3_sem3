#include <iostream>

#include "..\include\smrt_ptr\WeakPtr.h"
#include "..\include\smrt_ptr\UniquePtr.h"
#include "..\include\smrt_ptr\SharedPtr.h"
#include "..\include\sequence\Sequence.h"

#include "lib/googletest/include/gtest/gtest.h"

TEST(SharedPtr, BasicOperations) {
    SharedPtr<int> sp(new int(5));
    ASSERT_EQ(*sp, 5);
    ASSERT_EQ(sp.use_count(), 1);
}

TEST(SharedPtr, CopyConstructor) {
    SharedPtr<int> sp1(new int(10));
    SharedPtr<int> sp2(sp1);
    ASSERT_EQ(*sp1, 10);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp1.use_count(), 2);
    ASSERT_EQ(sp2.use_count(), 2);
}

TEST(SharedPtr, AssignmentOperator) {
    SharedPtr<int> sp1(new int(15));
    SharedPtr<int> sp2(new int(20));
    sp1 = sp2;
    ASSERT_EQ(*sp1, 20);
    ASSERT_EQ(*sp2, 20);
    ASSERT_EQ(sp1.use_count(), 2);
    ASSERT_EQ(sp2.use_count(), 2);
}

TEST(SharedPtr, Destructor) {
    SharedPtr<int> sp1(new int(25));
    {
        SharedPtr<int> sp2(sp1);
        ASSERT_EQ(*sp1, 25);
        ASSERT_EQ(*sp2, 25);
        ASSERT_EQ(sp1.use_count(), 2);
        ASSERT_EQ(sp2.use_count(), 2);
    }
    ASSERT_EQ(sp1.use_count(), 1);
}

TEST(SharedPtr, MultipleReferences) {
    SharedPtr<int> sp1(new int(30));
    SharedPtr<int> sp2(sp1);
    SharedPtr<int> sp3(sp1);
    ASSERT_EQ(*sp1, 30);
    ASSERT_EQ(*sp2, 30);
    ASSERT_EQ(*sp3, 30);
    ASSERT_EQ(sp1.use_count(), 3);
    ASSERT_EQ(sp2.use_count(), 3);
    ASSERT_EQ(sp3.use_count(), 3);
    sp2.reset(); // Уменьшаем счетчик ссылок
    ASSERT_EQ(sp1.use_count(), 2);
    ASSERT_EQ(sp3.use_count(), 2);
}

TEST(SharedPtr, Reset) {
    SharedPtr<int> sp(new int(35));
    ASSERT_EQ(*sp, 35);
    ASSERT_EQ(sp.use_count(), 1);
    sp.reset();
    ASSERT_EQ(sp.use_count(), 0); // Счетчик ссылок должен быть нулевым
    ASSERT_EQ(sp.get(), nullptr); // `sp` теперь должен быть недействительным
}

TEST(SharedPtr, MoveConstructor) {
    SharedPtr<int> sp1(new int(40));
    SharedPtr<int> sp2(std::move(sp1));
    ASSERT_EQ(*sp2, 40);
    ASSERT_EQ(sp1.get(), nullptr); // `sp1` больше не ссылается на объект
    ASSERT_EQ(sp2.use_count(), 1); // `sp2` теперь владеет объектом
}

TEST(SharedPtr, MoveAssignmentOperator) {
    SharedPtr<int> sp1(new int(45));
    SharedPtr<int> sp2(new int(50));
    sp2 = std::move(sp1);
    ASSERT_EQ(*sp2, 45);
    ASSERT_EQ(sp1.get(), nullptr); // `sp1` больше не ссылается на объект
    ASSERT_EQ(sp2.use_count(), 1); // `sp2` теперь владеет объектом
}

TEST(SharedPtr, WeakPtr) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    ASSERT_EQ(sp1.use_count(), 1);
    ASSERT_EQ(wp.use_count(), 1);

    sp1.reset();
    ASSERT_EQ(sp1.use_count(), 0);
    ASSERT_EQ(wp.use_count(), 0);

    ASSERT_EQ(wp.lock().get(), nullptr);
}

TEST(SharedPtr, Swap) {
    SharedPtr<int> sp1(new int(60));
    SharedPtr<int> sp2(new int(70));
    sp1.swap(sp2);
    ASSERT_EQ(*sp1, 70);
    ASSERT_EQ(*sp2, 60);
}

TEST(SharedPtr, NullPtrException) {
    SharedPtr<int> sp;
    ASSERT_EQ(sp.get(), nullptr);
    try {
        *sp;
        FAIL() << "Expected Null pointer exception";
    }
    catch (NullPointerException &e) {
        EXPECT_EQ(e.what(), std::string("Dereferencing null pointer"));
    }
}

TEST(UniquePtr, DefaultConstructor) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(UniquePtr, ConstructorWithPointer) {
    UniquePtr<int> ptr(new int(5));
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 5);
}

TEST(UniquePtr, MoveConstructor) {
    UniquePtr<int> ptr1(new int(10));
    UniquePtr<int> ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_NE(ptr2.get(), nullptr);
    EXPECT_EQ(*ptr2, 10);
}

TEST(UniquePtr, MoveAssignment) {
    UniquePtr<int> ptr1(new int(20));
    UniquePtr<int> ptr2;
    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_NE(ptr2.get(), nullptr);
    EXPECT_EQ(*ptr2, 20);
}

TEST(UniquePtr, Reset) {
    UniquePtr<int> ptr(new int(30));
    ptr.reset(new int(40));

    EXPECT_EQ(*ptr, 40);
}

TEST(UniquePtr, Release) {
    UniquePtr<int> ptr(new int(50));
    int *rawPtr = ptr.release();

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*rawPtr, 50);

    delete rawPtr;
}

TEST(UniquePtr, Swap) {
    UniquePtr<int> ptr1(new int(60));
    UniquePtr<int> ptr2(new int(70));

    ptr1.swap(ptr2);

    EXPECT_EQ(*ptr1, 70);
    EXPECT_EQ(*ptr2, 60);
}

TEST(UniquePtr, SwapWithRawPointer) {
    UniquePtr<int> ptr(new int(80));
    int *rawPtr = new int(90);

    ptr.swap(rawPtr);

    EXPECT_EQ(*ptr, 90);

    delete rawPtr;
}

TEST(UniquePtr, NullPtrException) {
    UniquePtr<int> sp;
    ASSERT_EQ(sp.get(), nullptr);
    try {
        *sp;
        FAIL() << "Expected Null pointer exception";
    }
    catch (NullPointerException &e) {
        EXPECT_EQ(e.what(), std::string("Null pointer exception"));
    }
}

TEST(WeakPtr, ConstructorFromSharedPtr) {
    SharedPtr<int> sp1(new int(50));
    WeakPtr<int> wp(sp1);
    EXPECT_EQ(wp.use_count(), 1);
}

TEST(WeakPtr, CopyConstructor) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    WeakPtr<int> copy(wp);
    EXPECT_EQ(copy.use_count(), 1);
}

TEST(WeakPtr, MoveConstructor) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    WeakPtr<int> moved(std::move(wp));
    EXPECT_EQ(moved.use_count(), 1);
}

TEST(WeakPtr, LockReturnsSharedPtr) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    SharedPtr<int> locked = wp.lock();
    EXPECT_NE(locked.get(), nullptr);
    EXPECT_EQ(*locked.get(), 55);
}

TEST(WeakPtr, Reset) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    wp.reset();
    EXPECT_EQ(wp.use_count(), 0);
}

TEST(WeakPtr, Swap) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp1(sp1);
    WeakPtr<int> wp2(sp1);

    wp1.swap(wp2);

    EXPECT_EQ(wp1.use_count(), 1);
    EXPECT_EQ(wp2.use_count(), 1);
}

TEST(WeakPtr, Destructor) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> wp(sp1);
    {
        WeakPtr<int> wp1(sp1);
        EXPECT_EQ(wp.weak_count(), 2); // Проверяем, что счетчик слабых ссылок увеличился
    }
    EXPECT_EQ(wp.weak_count(), 1); // Проверяем, что счетчик слабых ссылок уменьшился при выходе из области видимости
}

TEST(WeakPtr, LockReturnsNullptrWhenExpired) {
    SharedPtr<int> sp1(new int(55));
    WeakPtr<int> weakPtr(sp1);
    sp1.reset(); // Сбрасываем shared_ptr, чтобы он больше не указывал на объект
    SharedPtr<int> locked = weakPtr.lock();
    EXPECT_EQ(locked.get(), nullptr);
}

//TEST(SharedPtr, Upcasting) {
//    SharedPtr<LinkedListSequence<int>> linkedListPtr(new LinkedListSequence<int>());
//
//    SharedPtr<Sequence<int>> sequencePtr = linkedListPtr; // Upcasting
//
//    sequencePtr->append(42);
//    EXPECT_EQ(sequencePtr->get(0), 42);
//}

//TEST(SharedPtr, UpcastingReferenceCount) {
//    SharedPtr<LinkedListSequence<int>> linkedListPtr(new LinkedListSequence<int>());
//    EXPECT_EQ(linkedListPtr.use_count(), 1); // Проверяем, что счётчик ссылок равен 1
//
//    SharedPtr<Sequence<int>> sequencePtr = linkedListPtr; // Upcasting
//    EXPECT_EQ(linkedListPtr.use_count(), 2); // Проверяем, что счётчик ссылок равен 2 после апкаста
//
//    sequencePtr->append(10);
//    EXPECT_EQ(sequencePtr->get(0), 10); // Проверяем, что новый элемент добавился
//}
//
//TEST(SharedPtr, UpcastingDestructor) {
//    {
//        SharedPtr<LinkedListSequence<int>> linkedListPtr(new LinkedListSequence<int>());
//        SharedPtr<Sequence<int>> sequencePtr = linkedListPtr; // Upcasting
//        EXPECT_EQ(linkedListPtr.use_count(), 2); // Счётчик ссылок равен 2
//    } // linkedListPtr и sequencePtr выходят из области видимости
//
//}

//TEST(SharedPtr, UpcastingInvalidType) {
//    SharedPtr<LinkedListSequence<int>> linkedListPtr(new LinkedListSequence<int>());
//
//}
//
//TEST(SharedPtr, UpcastingSamePointer) {
//    SharedPtr<LinkedListSequence<int>> linkedListPtr(new LinkedListSequence<int>());
//
//    SharedPtr<Sequence<int>> sequencePtr1 = linkedListPtr; // Upcasting
//    SharedPtr<Sequence<int>> sequencePtr2 = linkedListPtr; // Ещё один upcasting
//
//    EXPECT_EQ(linkedListPtr.use_count(), 3); // Счётчик ссылок должен быть 3
//}
