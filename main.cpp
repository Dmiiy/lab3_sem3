#include <iostream>
#include <vector>
#include <string>

void printNames(const std::vector<std::string>& names) {
    for (const auto& name : names) {
        std::cout << "Name: " << name << std::endl;
    }
}

int main() {
    std::vector<std::string> names = {"John Doe", "Jane Smith", "Alice Johnson"};
    printNames(names);

    return 0;
}