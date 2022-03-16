//
// Created by stian on 04.02.2022.
//

#include "student.h"
#include <string>
#include <iostream>

Student::Student(std::string name, std::string email)
{
    id= current_id++;
    this->name= std::move(name);
    this->email= std::move(email);
}

int Student::GetId() const
{
    return id;
}

std::string Student::GetName() const
{
    return name;
}

void Student::SetName(std::string name)
{
    this->name= std::move(name);
}

std::string Student::GetEmail() const
{
    return email;
}

void Student::SetEmail(std::string email)
{
    this->email= std::move(email);
}

void Student::Print() const
{
    std::cout << "student id = " << id << ", name = " << name << ", email = " << email << std::endl;
}

int Student::current_id= 1;