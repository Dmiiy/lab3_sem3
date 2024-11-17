#include <gtest/gtest.h>
#include "../include/sorting/BubbleSort.h"
#include "../include/sorting/SelectionSort.h"
#include "../include/sorting/CountingSort.h"
#include "../include/sorting/InsertionSort.h"
#include "../include/sorting/MergeSort.h"
#include "../include/sorting/QuickSorter.h"
#include "../include/sorting/ShakerSort.h"
#include "../include/sorting/BinaryInsertionSort.h"
#include "../include/sorting/HeapSort.h"
#include "../include/sorting/ShellSort.h"
#include "../include/sorting/BatcherSort.h"
#include "../include/sorting/ShellSortGap.h"

TEST(BubbleSort, BasicSort) {
    ArraySequence<int> list;
    list.append(3);
    list.append(1);
    list.append(4);
    list.append(1);
    list.append(5);

    BubbleSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 1);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 4);
    ASSERT_EQ(list.get(4), 5);
}

TEST(BubbleSort, EmptyList) {
    ArraySequence<int> list;
    BubbleSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.getLength(), 0); // Проверяем, что длина списка не изменилась
}

TEST(BubbleSort, SingleElement) {
    ArraySequence<int> list;
    list.append(42);

    BubbleSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 42);
    ASSERT_EQ(list.getLength(), 1);
}

TEST(ShakerSort, BasicSort) {
    ArraySequence<int> list;
    list.append(9);
    list.append(7);
    list.append(5);
    list.append(3);
    list.append(1);

    ShakerSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 3);
    ASSERT_EQ(list.get(2), 5);
    ASSERT_EQ(list.get(3), 7);
    ASSERT_EQ(list.get(4), 9);
}

TEST(ShakerSort, ReverseSort) {
    ArraySequence<int> list;
    list.append(1);
    list.append(3);
    list.append(5);
    list.append(7);
    list.append(9);

    ShakerSorter<int> sorter([](const int &a, const int &b) { return a > b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 9);
    ASSERT_EQ(list.get(1), 7);
    ASSERT_EQ(list.get(2), 5);
    ASSERT_EQ(list.get(3), 3);
    ASSERT_EQ(list.get(4), 1);
}

TEST(InsertionSort, BasicSort) {
    ArraySequence<int> list;
    list.append(10);
    list.append(2);
    list.append(8);
    list.append(4);
    list.append(6);

    InsertionSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 2);
    ASSERT_EQ(list.get(1), 4);
    ASSERT_EQ(list.get(2), 6);
    ASSERT_EQ(list.get(3), 8);
    ASSERT_EQ(list.get(4), 10);
}

TEST(SelectionSort, BasicSort) {
    ArraySequence<int> list;
    list.append(7);
    list.append(2);
    list.append(5);
    list.append(3);
    list.append(1);

    SelectionSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 5);
    ASSERT_EQ(list.get(4), 7);
}

TEST(MergeSort, BasicSort) {
    ArraySequence<int> list;
    list.append(8);
    list.append(4);
    list.append(5);
    list.append(1);
    list.append(7);

    MergeSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 4);
    ASSERT_EQ(list.get(2), 5);
    ASSERT_EQ(list.get(3), 7);
    ASSERT_EQ(list.get(4), 8);
}

TEST(MergeSort, AlreadySorted) {
    ArraySequence<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    MergeSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 4);
    ASSERT_EQ(list.get(4), 5);
}

TEST(CountingSort, BasicSort) {
    ArraySequence<int> list;
    list.append(5);
    list.append(1);
    list.append(4);
    list.append(2);
    list.append(3);

    CountingSorter<int,int> sorter([](const int &a, const int &b) { return a < b; },intExtractor);
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 4);
    ASSERT_EQ(list.get(4), 5);
}

TEST(BinaryInsertionSort, BasicSort) {
    ArraySequence<int> list;
    list.append(7);
    list.append(3);
    list.append(5);
    list.append(2);
    list.append(9);

    BinaryInsertionSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 2);
    ASSERT_EQ(list.get(1), 3);
    ASSERT_EQ(list.get(2), 5);
    ASSERT_EQ(list.get(3), 7);
    ASSERT_EQ(list.get(4), 9);
}

TEST(QuickSort, BasicSort) {
    ArraySequence<int> list;
    list.append(9);
    list.append(4);
    list.append(6);
    list.append(3);
    list.append(1);

    QuickSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 3);
    ASSERT_EQ(list.get(2), 4);
    ASSERT_EQ(list.get(3), 6);
    ASSERT_EQ(list.get(4), 9);
}

TEST(HeapSort, BasicSort) {
    ArraySequence<int> list;
    list.append(4);
    list.append(10);
    list.append(3);
    list.append(5);
    list.append(1);

    HeapSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 3);
    ASSERT_EQ(list.get(2), 4);
    ASSERT_EQ(list.get(3), 5);
    ASSERT_EQ(list.get(4), 10);
}

TEST(HeapSort, EmptyList) {
    ArraySequence<int> list;
    HeapSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.getLength(), 0); // Проверяем, что длина списка не изменилась
}

TEST(HeapSort, SingleElement) {
    ArraySequence<int> list;
    list.append(42);

    HeapSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 42);
    ASSERT_EQ(list.getLength(), 1);
}

TEST(ShellSort, BasicSort) {
    ArraySequence<int> list;
    list.append(8);
    list.append(5);
    list.append(2);
    list.append(6);
    list.append(3);

    ShellSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 2);
    ASSERT_EQ(list.get(1), 3);
    ASSERT_EQ(list.get(2), 5);
    ASSERT_EQ(list.get(3), 6);
    ASSERT_EQ(list.get(4), 8);
}

TEST(ShellSort, AlreadySorted) {
    ArraySequence<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    ShellSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 4);
    ASSERT_EQ(list.get(4), 5);
}

TEST(ShellSortGap, BasicSort) {
    ArraySequence<int> list;
    list.append(20);
    list.append(35);
    list.append(15);
    list.append(40);
    list.append(10);

    // Определим последовательность разностей для сортировки Шелла
    ArraySequence<int> gapSequence;
    gapSequence.append(3); // Пример разностей: сначала 3, затем 1
    gapSequence.append(1);

    // Передаем разности и компаратор в конструктор сортировщика
    ShellSorterWithGap<int> sorter(gapSequence, [](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 10);
    ASSERT_EQ(list.get(1), 15);
    ASSERT_EQ(list.get(2), 20);
    ASSERT_EQ(list.get(3), 35);
    ASSERT_EQ(list.get(4), 40);
}

TEST(ShellSortGap, EmptyList) {
    ArraySequence<int> list;

    // Определим пустую последовательность разностей, так как список пуст
    ArraySequence<int> gapSequence;

    ShellSorterWithGap<int> sorter(gapSequence, [](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.getLength(), 0); // Проверяем, что длина списка не изменилась
}

TEST(ShellSortGap, SingleElement) {
    ArraySequence<int> list;
    list.append(42);

    // Последовательность разностей для одного элемента не важна
    ArraySequence<int> gapSequence;
    gapSequence.append(1);

    ShellSorterWithGap<int> sorter(gapSequence, [](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 42);
    ASSERT_EQ(list.getLength(), 1);
}


TEST(BatcherSort, BasicSort) {
    ArraySequence<int> list;
    list.append(4);
    list.append(2);
    list.append(8);
    list.append(6);
    list.append(1);

    BatcherSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 4);
    ASSERT_EQ(list.get(3), 6);
    ASSERT_EQ(list.get(4), 8);
}

TEST(BatcherSort, AlreadySorted) {
    ArraySequence<int> list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    BatcherSorter<int> sorter([](const int &a, const int &b) { return a < b; });
    sorter.sort(&list);

    ASSERT_EQ(list.get(0), 1);
    ASSERT_EQ(list.get(1), 2);
    ASSERT_EQ(list.get(2), 3);
    ASSERT_EQ(list.get(3), 4);
    ASSERT_EQ(list.get(4), 5);
}
