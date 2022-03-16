#include <iostream>
#include <vector>
#include <string>
#include "student.h"
#include "test.h"
#include "testresult.h"


void menu() {
    std::cout << "1. Add student\n2. Edit student\n3. Remove student\n\n4. Add test\n5. Edit test\n6. Remove test\n\n"
                 "7. Add test result\n8. Edit test result\n9. Remove test result\n\n10. Show all info\n11. Exit\n";
}

int choose_student(const std::vector<Student>& students) {
    if (students.empty()) return 0;

    for (auto & student : students) std::cout << student.GetId() << " " << student.GetName()
    << " " << student.GetEmail() << std::endl;

    int choice;
    std::cin >> choice;

    return choice;
}

int choose_test(const std::vector<Test>& tests) {
    if (tests.empty()) return 0;

    for (auto & test : tests) std::cout << test.GetId() << " " << test.GetName()
    << " " << test.GetCourse() << " " << test.GetDate() << std::endl;

    int choice;
    std::cin >> choice;

    return choice;
}

int main() {
    std::vector<Student> students;
    std::vector<Test> tests;
    std::vector<TestResult> results;

    int option = 0;
    while (option != 11) {
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

                std::string name;
                std::string email;
                getchar();
                std::getline(std::cin, name);
                std::getline(std::cin, email);
                Student student(name,email);
                students.push_back(student);
                break;
            }

            case 2:
            {
                int choice= choose_student(students);
                if (choice == students.size()+1) break;

                if (choice == 0)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                std::string name;
                std::string email;

                getchar();
                std::getline(std::cin, name);
                std::getline(std::cin, email);

                students[choice-1].SetName(name);
                students[choice-1].SetEmail(email);
                break;
            }

            case 3:
            {
                int choice= choose_student(students);
                if (choice == students.size()+1) break;

                if (choice == 0)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                students.erase(students.begin()+choice-1);
                break;
            }

            case 4:
            {
                std::string name;
                std::string course;
                std::string date;

                getchar();
                std::getline(std::cin, course);
                std::getline(std::cin, name);
                std::getline(std::cin, date);

                Test test(name, course, date);
                tests.push_back(test);
                break;
            }

            case 5:
            {
                int choice= choose_test(tests);
                if (choice == tests.size()+1) break;

                if (choice == 0)
                {
                    std::cout << "No tests found" << std::endl;
                    break;
                }

                std::string name;
                std::string course;
                std::string date;

                getchar();
                std::getline(std::cin, course);
                std::getline(std::cin, name);
                std::getline(std::cin, date);

                tests[choice-1].SetName(name);
                tests[choice-1].SetCourse(course);
                tests[choice-1].SetDate(date);
                break;
            }

            case 6:
            {
                int choice= choose_test(tests);
                if (choice == tests.size()+1) break;

                if (choice == 0)
                {
                    std::cout << "No tests found" << std::endl;
                    break;
                }

                tests.erase(tests.begin()+choice-1);
                break;
            }

            case 7:
            {
                int student_choice= choose_student(students);
                int test_choice= choose_test(tests);

                int grade;
                std::cin >> grade;

                TestResult result(grade, &students[student_choice-1], &tests[test_choice-1]);
                results.push_back(result);
                break;
            }

            case 8:
            {
                int student_choice= choose_student(students);
                if (student_choice == students.size()+1) break;

                if (student_choice == 0)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                int test_choice= choose_test(tests);
                if (test_choice == tests.size()+1) break;

                if (test_choice == 0)
                {
                    std::cout << "No tests found" << std::endl;
                    break;
                }

                int grade;
                std::cin >> grade;

                Student* student= &students[student_choice-1];
                Test *test= &tests[test_choice-1];

                for (auto &result : results)
                {
                    if (result.GetStudent() == student)
                    {
                        if (result.GetTest() == test)
                        {
                            result.SetGrade(grade);
                            break;
                        }
                    }
                }
                break;
            }

            case 9:
            {
                int student_choice= choose_student(students);
                if (student_choice == students.size()+1) break;

                if (student_choice == 0)
                {
                    std::cout << "No students found" << std::endl;
                    break;
                }

                int test_choice= choose_test(tests);
                if (test_choice == tests.size()+1) break;

                if (test_choice == 0)
                {
                    std::cout << "No tests found" << std::endl;
                    break;
                }

                Student* student= &students[student_choice-1];
                Test *test= &tests[test_choice-1];

                int i= 0;
                for (auto & result : results)
                {
                    if (result.GetStudent() == student)
                    {
                        if (result.GetTest() == test)
                        {
                            results.erase(results.begin()+i);
                            break;
                        }
                    }
                    i++;
                }
                break;
            }

            case 10:
            {
                std::cout << "Student details:" << std::endl;
                for (auto &student : students) student.Print();

                std::cout << std::endl << "Test details:" << std::endl;
                for (auto &test : tests) test.Print();

                std::cout << std::endl << "Test results:" << std::endl;
                for (auto &result : results) result.Print();
                break;
            }
            case 11:
                break;
        }
    }
    return 0;
}
