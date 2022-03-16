//
// Created by stian on 14.02.2022.
//

#ifndef ASSIGNMENT_4_2_STUDENT_H
#define ASSIGNMENT_4_2_STUDENT_H
#include <string>
#include <utility>

class Student {
public:
    int id;
    std::string name;
    int age;
    int attendance;
    Student(int id, std::string name, int age, int attendance);
};


#endif //ASSIGNMENT_4_2_STUDENT_H
