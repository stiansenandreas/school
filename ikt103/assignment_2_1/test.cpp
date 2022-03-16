//
// Created by stian on 04.02.2022.
//

#include "test.h"
#include <string>
#include <iostream>

Test::Test(std::string name, std::string course, std::string date)
{
    id= current_id++;
    this->name= std::move(name);
    this->course= std::move(course);
    this->date= std::move(date);
}

int Test::GetId() const
{
    return id;
}

std::string Test::GetName() const
{
    return name;
}

void Test::SetName(std::string name)
{
    this->name= std::move(name);
}

std::string Test::GetCourse() const
{
    return course;
}

void Test::SetCourse(std::string course)
{
    this->course= std::move(course);
}

std::string Test::GetDate() const
{
    return date;
}

void Test::SetDate(std::string date)
{
    this->date= std::move(date);
}

void Test::Print() const
{
    std::cout << "test id = " << id << ", course name = " << course << ", test name = " << name
    << ", date = " << date << std::endl;
}

int Test::current_id= 1;