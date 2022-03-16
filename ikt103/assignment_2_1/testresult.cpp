//
// Created by stian on 10.02.2022.
//

#include <string>
#include <iostream>
#include "student.h"
#include "test.h"
#include "testresult.h"

TestResult::TestResult(int grade, Student *student, Test *test) : student(student), test(test)
{
    this->grade=grade;
}

int TestResult::GetGrade() const
{
    return grade;
}

void TestResult::SetGrade(int grade)
{
    this->grade= grade;
}

const Student *TestResult::GetStudent() const
{
    return student;
}

const Test *TestResult::GetTest() const
{
    return test;
}

void TestResult::Print() const
{
    std::cout << "student id = " << student->GetId() << ", course name = " << test->GetCourse() << ", test id = "
    << test->GetId() << ", test name = " << test->GetName() << ", grade = " << grade << std::endl;
}

void TestResult::ShortPrint() const
{
    std::cout << student->GetId() << " " << test->GetCourse() << " " << test->GetId() << " " << test->GetName() << " "
        << grade << std::endl;
}