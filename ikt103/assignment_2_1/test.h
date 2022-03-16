//
// Created by stian on 04.02.2022.
//

#ifndef ASSIGNMENT_2_TEST_H
#define ASSIGNMENT_2_TEST_H
#include <iostream>
#include <string>

class Test {
protected:
    int id;
    static int current_id;
    std::string name;
    std::string course;
    std::string date;

public:
    Test(std::string name, std::string course, std::string date);

    int GetId() const;

    std::string GetName() const;

    void SetName(std::string name);

    std::string GetCourse() const;

    void SetCourse(std::string course);

    std::string GetDate() const;

    void SetDate(std::string date);

    void Print() const;
};


#endif //ASSIGNMENT_2_TEST_H
