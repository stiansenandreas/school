#include <iostream>
#include "student.h"
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>


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
    SQLite::Database studentDB("school.sqlite", SQLite::OPEN_READWRITE);

    int option = 0;
    while (option != 7) {
        menu();

        std::cin >> option;
        switch (option)
        {
            default:
            {
                std::cout << "Invalid option" << std::endl;
                break;
            }

            case 1:
            {
                SQLite::Statement query(studentDB, "SELECT * FROM students");
                if (query.tryExecuteStep() != SQLITE_ROW)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                query.reset();
                while (query.tryExecuteStep() == SQLITE_ROW)
                {
                    Student student(query.getColumn("id"),
                                    query.getColumn("name"),
                                    query.getColumn("email"),
                                    query.getColumn("year"));
                    student.Print();
                }
                break;
            }

            case 2:
            {
                SQLite::Statement query(studentDB, "SELECT * FROM students WHERE id=?");
                int inputId;
                std::cin >> inputId;
                query.bind(1, inputId);

                if (query.tryExecuteStep() != SQLITE_ROW)
                {
                    std::cout << "Student not found" << std::endl;
                    break;
                }

                Student student(query.getColumn("id"),
                                query.getColumn("name"),
                                query.getColumn("email"),
                                query.getColumn("year"));
                student.Print();
                break;
            }

            case 3:
            {
                SQLite::Statement query(studentDB, "INSERT INTO students (name, email, year) VALUES (?, ?, ?)");
                std::string name;
                getchar();
                std::getline(std::cin, name);
                query.bind(1, name);
                std::string email;
                std::cin >> email;
                query.bind(2, email);
                int year;
                std::cin >> year;
                query.bind(3, year);
                query.executeStep();

                SQLite::Statement query2(studentDB, "SELECT id FROM students ORDER BY id DESC LIMIT 1");
                query2.executeStep();

                Student student(query2.getColumn("id"), name, email, year);
                std::cout << "Added student: ";
                student.Print();
                break;
            }

            case 4:
            {
                int id;
                std::cin >> id;

                SQLite::Statement query0(studentDB, "SELECT * FROM students WHERE id = ?");
                query0.bind(1, id);
                if (query0.tryExecuteStep() != SQLITE_ROW)
                {
                    std::cout << "Student not found" << std::endl;
                    break;
                }

                std::string name;
                getchar();
                std::getline(std::cin, name);
                std::string email;
                std::cin >> email;
                int year;
                std::cin >> year;

                SQLite::Statement query(studentDB, "UPDATE students SET name = ?, email = ?, year = ? WHERE id = ?");
                query.bind(1, name);
                query.bind(2, email);
                query.bind(3, year);
                query.bind(4, id);
                query.tryExecuteStep();

                std::cout << "Student was edited successfully" << std::endl;
                break;
            }

            case 5:
            {
                int id;
                std::cin >> id;
                SQLite::Statement query(studentDB, "DELETE FROM students WHERE id = ?");
                query.bind(1, id);

                if (query.exec() == 0)
                {
                    std::cout << "Student not found" << std::endl;
                    break;
                }

                else
                {
                    std::cout << "Student was removed successfully" << std::endl;
                    break;
                }
            }

            case 6:
            {
                std::string name;
                std::cin >> name;
                SQLite::Statement query(studentDB, "SELECT * FROM students WHERE name LIKE ?");
                query.bind(1, name);

                if (query.tryExecuteStep() != SQLITE_ROW)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                else
                {
                    query.reset();
                    while (query.tryExecuteStep() == SQLITE_ROW)
                    {
                        Student student(query.getColumn("id"),
                                        query.getColumn("name"),
                                        query.getColumn("email"),
                                        query.getColumn("year"));
                        student.Print();
                    }
                    break;
                }
            }

            case 7:
                break;
        }


    }
    return 0;
}
