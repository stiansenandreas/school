#include <iostream>
#include <filesystem>
#include "student.h"
#include "sqlite3/sqlite3.h"
#include "sqlite_orm/sqlite_orm.h"

namespace fs = std::filesystem;
using namespace sqlite_orm;

void menu() {
    std::cout << "1. Read all students\n"
                 "2. Get student by id\n"
                 "3. Add student\n"
                 "4. Edit student\n"
                 "5. Remove student\n"
                 "6. Search\n"
                 "7. Exit\n";
}


int main() {

    auto storage = make_storage("school.sqlite",
                                    make_table("students",
                                           make_column("id", &Student::id, autoincrement(), primary_key()),
                                           make_column("name", &Student::name),
                                           make_column("email", &Student::email),
                                           make_column("year", &Student::year)));

    int option = 0;
    while (option != 7) {
        menu();

        std::cin >> option;
        getchar();
        switch (option)
        {
            default:
            {
                std::cout << "Invalid option" << std::endl;
                break;
            }

            case 1:
            {
                auto students = storage.get_all<Student>();

                if (students.empty())
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                for (auto &student: students)
                {
                    std::cout << "id: " << student.id << ", name: " << student.name << ", email: "
                              << student.email << ", year: " << student.year << std::endl;
                }
                break;
            }

            case 2:
            {
                int inputId;
                std::cin >> inputId;
                try {
                    auto student = storage.get<Student>(inputId);
                    std::cout << "id: " << student.id << ", name: " << student.name << ", email: "
                              << student.email << ", year: " << student.year << std::endl;
                    break;
                }
                catch (std::system_error &error)
                {
                    std::cout << "Student not found" << std::endl;
                    break;
                }


                break;
            }

            case 3:
            {
                std::string name;
                std::getline(std::cin, name);
                std::string email;
                std::cin >> email;
                int year;
                std::cin >> year;

                Student student = { -1, name, email, year};
                student.id = storage.insert(student);
                std::cout << "Added student: ";
                std::cout << "id: " << student.id << ", name: " << student.name << ", email: "
                          << student.email << ", year: " << student.year << std::endl;
                break;
            }

            case 4:
            {
                int inputId;
                std::cin >> inputId;
                getchar();

                try {
                    auto student = storage.get<Student>(inputId);

                    std::string name;
                    std::getline(std::cin, name);
                    std::string email;
                    std::cin >> email;
                    int year;
                    std::cin >> year;

                    student.name = name;
                    student.email = email;
                    student.year = year;
                    storage.update(student);

                    std::cout << "Student was edited successfully" << std::endl;
                    break;
                }

                catch (std::system_error &error)
                {
                    std::cout << "Student not found" << std::endl;
                    break;
                }
            }

            case 5: {
                int inputId;
                std::cin >> inputId;
                getchar();

                try {
                    auto student = storage.get<Student>(inputId);
                    storage.remove<Student>(student.id);

                    std::cout << "Student was removed successfully" << std::endl;
                    break;
                }

                catch (std::system_error &error) {
                    std::cout << "Student not found" << std::endl;
                    break;
                }
            }

            case 6: {
                std::string name;
                std::cin >> name;
                getchar();

                try {
                    auto students = storage.get_all<Student>(where(like(&Student::name, name)));
                    for (auto &student: students) {
                        std::cout << "id: " << student.id << ", name: " << student.name << ", email: "
                                  << student.email << ", year: " << student.year << std::endl;
                    }
                    break;
                }

                catch (std::system_error &error) {
                    std::cout << "Student not found" << std::endl;
                    break;
                }
        }

            case 7:
                break;
        }

    }
    return 0;
}
