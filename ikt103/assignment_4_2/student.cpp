//
// Created by stian on 15.02.2022.
//
#include "student.h"

Student::Student(int id, std::string name, int age, int attendance)
{
    this->id = id;
    this->name = std::move(name);
    this->age = age;
    this->attendance = attendance;
}