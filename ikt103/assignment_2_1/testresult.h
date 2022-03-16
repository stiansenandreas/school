//
// Created by stian on 10.02.2022.
//

#ifndef MAIN_CPP_TESTRESULT_H
#define MAIN_CPP_TESTRESULT_H
#include <string>
#include <iostream>
#include "student.h"
#include "test.h"

class TestResult{
protected:
    int grade;
    Student *student;
    Test *test;

public:
    TestResult(int grade, Student *student, Test *test);

        int GetGrade() const;

        void SetGrade(int grade);

        const Student *GetStudent() const;

        const Test *GetTest() const;

        void Print() const;

        void ShortPrint() const;
    };


#endif //MAIN_CPP_TESTRESULT_H
