#include <iostream>
#include <fstream>
#include <list>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "student.h"


int main() {
    std::ifstream ifs("students.json");
    if (!ifs.is_open())
    {
        std::cout << "Error: Failed to open file. Exiting" << std::endl;
        return 1;
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    auto array = doc.GetArray();
    std::list<Student*> students;

    for (auto &obj : array)
    {
        auto studentObj = obj.GetObj();
        students.push_back(new Student(studentObj["id"].GetInt(),
                                       studentObj["name"].GetString(),
                                       studentObj["age"].GetInt(),
                                       studentObj["attendance"].GetInt() ));
    }

    Student* youngest = students.front();
    Student* oldest = students.front();
    int sumAge = 0;

    for (auto &student : students)
    {
        std::cout << student->name << std::endl;

        if (student->age < youngest->age)
            youngest = student;

        if (student->age > oldest->age)
            oldest = student;

        sumAge += student->age;

        if (student->attendance < 30)
            std::cout << "Bad student: " << student->name << std::endl;
    }

    std::cout << "Youngest: " << youngest->name << std::endl;
    std::cout << "Oldest: " << oldest->name << std::endl;
    std::cout << "Average age: " << sumAge/int(students.size()) << std::endl;
    return 0;
}
