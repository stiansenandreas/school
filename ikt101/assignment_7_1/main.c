#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 50

typedef struct
{
    int age;
    char name[STR_LEN+1];
} student_info;


student_info get_info()
{
    student_info student= {0};
    fgets(student.name, STR_LEN+1, stdin);
    student.name[strlen(student.name)-1]= 0;

    if (strcmp(student.name, "stop") == 0)
        return student;

    scanf("%d", &student.age);
    getchar();
    return student;
}


int main()
{
    student_info *students= NULL;
    students= malloc(sizeof(student_info)*1);
    int count= 0;

    while(true)
    {
        if (count > 0) students= realloc(students,sizeof(student_info)*(count+1));
        students[count]= get_info();

        if (strcmp(students[count].name, "stop") == 0)
        {
            if (count == 0)
            {
                printf("No students were given");
                return 1;
            }
            break;
        }
        count++;
    }

    printf("Count: %d\n", count);
    int youngest= 0;
    int oldest= 0;

    for (int i= 0; i < count; i++)
    {
        printf("Name = %s, Age = %d\n", students[i].name, students[i].age);
        if (students[i].age > students[oldest].age)
            oldest=i;
        if (students[i].age < students[youngest].age)
            youngest=i;
    }

    printf("Youngest: %s\n", students[youngest].name);
    printf("Oldest: %s", students[oldest].name);
    free(students);
    return 0;
}
