#include <iostream>


int main() {
    std::string input_1;
    std::string input_2;
    std::getline(std::cin, input_1);
    std::getline(std::cin, input_2);

    if (input_1 == input_2)
        std::cout << "The strings are equal" << std::endl;
    else
        std::cout << "The strings are not equal" << std::endl;

    if (input_1.find(input_2) != -1)
        std::cout << "Last string is substring of first string" << std::endl;
    else if (input_2.find(input_1) != -1)
        std::cout << "First string is substring of last string" << std::endl;
    else
        std::cout << "There are no substrings" << std::endl;

    return 0;
