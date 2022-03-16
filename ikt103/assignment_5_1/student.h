//
// Created by stian on 04.02.2022.
//

#ifndef ASSIGNMENT_2_STUDENT_H
#define ASSIGNMENT_2_STUDENT_H
#include <string>
#include <iostream>

class Student {
protected:
    int id;
    static int current_id;
    std::string name;
    std::string email;
    int year;

public:
    Student(int id, std::string name, std::string email, int year);

    int GetId() const;

    std::string GetName() const;

    void SetName(std::string name);

    std::string GetEmail() const;

    void SetEmail(std::string email);

    int GetYear() const;

    void SetYear(int year);

    void Print() const;
};


#endif //ASSIGNMENT_2_STUDENT_H
