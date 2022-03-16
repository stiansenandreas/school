//
// Created by stian on 04.02.2022.
//

#include "student.h"
#include <string>
#include <iostream>

Student::Student(int id, std::string name, std::string email, int year)
{
    this->id = id;
    this->name = std::move(name);
    this->email = std::move(email);
    this->year = year;
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
    this->name = std::move(name);
}

std::string Student::GetEmail() const
{
    return email;
}

void Student::SetEmail(std::string email)
{
    this->email = std::move(email);
}

int Student::GetYear() const
{
    return year;
}

void Student::SetYear(int year)
{
    this->year = year;
}

void Student::Print() const
{
    std::cout << "id: " << id << ", name: " << name << ", email: " << email << ", year: " << year << std::endl;
}

int Student::current_id= 1;