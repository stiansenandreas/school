#include <iostream>
#include <vector>


class student
        {
        public:
            std::string name;
            std::string course;
            std::string grade;
        };


int main()
{
    int index= 0;
    std::vector<student> students;

    while(true)
    {
        students.resize(index+1);
        std::string input;
        std::getline(std::cin, input);
        if (input == "stop") break;
        students[index].name = input;

        while(true)
        {
            std::getline(std::cin, input);
            if (input == "stop") break;
            students[index].course = input;
            std::getline(std::cin, input);
            students[index].grade = input;
        }

        std::cout << students[index].name << " - " << students[index].course
        << " - " << students[index].grade << std::endl;
        index++;
    }

    return 0;
}
