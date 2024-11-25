#include <iostream>
#include "data_structures/BinaryTree.h"

int main() {
    // Создаём бинарное дерево
    BinaryTree<int> tree;

    // Вставляем значения
    std::cout << "Inserting values: 4, 2, 6, 1, 3, 5, 7" << std::endl;
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);

    // Печатаем минимальное и максимальное значение
    std::cout << "Minimum value: " << tree.getMin() << std::endl;
    std::cout << "Maximum value: " << tree.getMax() << std::endl;

    // Проверяем поиск
    int searchValue = 3;
    auto foundNode = tree.find(searchValue);
    if (foundNode) {
        std::cout << "Value " << searchValue << " found in the tree." << std::endl;
    } else {
        std::cout << "Value " << searchValue << " not found in the tree." << std::endl;
    }

    // Тестируем прошивку дерева
    std::cout << "Threading the tree in LNR (in-order) order..." << std::endl;
    tree.threadNLR();

    // Печатаем прошитое дерево
    std::cout << "Threaded tree values (LNR): ";
    tree.printThreaded();

    // Удаляем элемент
    int deleteValue = 6;
    std::cout << "Removing value " << deleteValue << "..." << std::endl;
    tree.remove(deleteValue);

    // Проверяем, что дерево всё ещё корректно
    std::cout << "Threading the tree after deletion..." << std::endl;
    tree.threadNLR();
    std::cout << "Threaded tree values after deletion: ";
    tree.printThreaded();

    // Итератор тестирование
    std::cout << "Using iterator to traverse the tree:" << std::endl;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
