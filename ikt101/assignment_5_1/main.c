#include <stdio.h>
#define STR_LEN 60

typedef struct {
    int id;
    char first_name[STR_LEN+1];
    char last_name[STR_LEN+1];
    int age;
} student_t;

int main() {
    student_t student;
    printf("Enter student details\n");
    scanf("%d %s %s %d", &student.id,student.first_name,student.last_name,&student.age);
    printf("Student id: %d\n", student.id);
    printf("Name: %s %s\n", student.first_name,student.last_name);
    printf("Age: %d\n", student.age);
    return 0;
}
