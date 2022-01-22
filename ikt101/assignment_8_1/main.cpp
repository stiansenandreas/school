#include <iostream>


class node {
public:
    int number;
    node *nxt_ptr;

    explicit node(int number) {
        this->number = number;
        nxt_ptr = nullptr;
    }
};



class list {
    node *head;
public:
    list() {head = nullptr; }


    void push_back(const int value) {
        size_t size = this->size();
        node *new_node = new node(value);

        if (size == 0) {
            head = new_node;
            return;
        }

        node *tmp = head;
        while (tmp->nxt_ptr != nullptr) tmp = tmp->nxt_ptr;
        tmp->nxt_ptr = new_node;
    }


    std::size_t size() {
        size_t size = 0;
        node *tmp = head;

        while (tmp != nullptr) {
            tmp = tmp->nxt_ptr;
            size++;
        }
        return size;
    }


    int& at(size_t index) {
        node *tmp = head;
        while (index-- > 0) tmp = tmp->nxt_ptr;
        return tmp->number;
    }


    void remove(size_t index) {
        size_t size = this->size();
        node *tmp1 = head, *tmp2 = nullptr;

        if (size == 0) {
            std::cout << "List contains no integers" << std::endl;
            return;
        }

        if (size < index) {
            std::cout << "Index out of range" << std::endl;
            return;
        }

        if (index == 0) {
            head = head->nxt_ptr;
            delete tmp1;
            return;
        }

        while (index-- > 0) {
            tmp2 = tmp1;
            tmp1 = tmp1->nxt_ptr;
        }

        tmp2->nxt_ptr = tmp1->nxt_ptr;
        delete tmp1;
    }
};



int main() {
    list myList;

    while (true) {
        std::cout << "1. Add integer" << std::endl << "2. Show integer at position" << std::endl <<
        "3. Remove integer at position" << std::endl << "4. Print information" << std::endl << "5. exit" << std::endl;

        int option; std::cin >> option;

        if (option == 1) {
            std::cout << "Enter integer" << std::endl;
            int value; std::cin >> value;
            myList.push_back(value);
            continue;
        }

        if (option == 2) {
            std::cout << "Enter index" << std::endl;
            size_t index; std::cin >> index;
            if (myList.size() == 0) {
                std::cout << "List contains no integers" << std::endl;
                continue;
            }
            std::cout << "Integer at index " << index << ": " << myList.at(index) << std::endl;
        }

        if (option == 3) {
            std::cout << "Enter index" << std::endl;
            size_t index; std::cin >> index;
            myList.remove(index);
        }

        if (option == 4) {
            if (myList.size() == 0) {
                std::cout << "List contains no integers" << std::endl;
                continue;
            }
            std::cout << "List contains " << myList.size() << " integers: ";
            for (int i= 0; i < myList.size(); i++)
                std::cout << myList.at(i) << " ";
            std::cout << std::endl;
        }

        if (option == 5) break;
    }

    return 0;
}
