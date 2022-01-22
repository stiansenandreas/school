#include <stdio.h>
#include <stdbool.h>

#define STR_LEN 50

typedef struct {
    char id[STR_LEN+1];
    char name[STR_LEN+1];
    char age[STR_LEN+1];
} student_t;


int main() {
    while(true)
    {
        printf("1. read file\n2. write to file\n3. exit\n");
        int option;
        scanf("%d", &option);
        getchar();
        if (option == 1)
        {
            FILE *student_file;
            student_t student;
            student_file= fopen("./student_read.txt", "r");

            fgets(student.id, sizeof(student.id), student_file);
            fgets(student.name, sizeof(student.name), student_file);
            fgets(student.age, sizeof(student.age), student_file);

            printf("Student id: %sName: %sAge: %s\n", student.id, student.name, student.age);
            fclose(student_file);
        }
        else if (option == 2)
        {
            FILE *student_file;
            student_t student;
            student_file= fopen("./student_write.txt", "w");

            fgets(student.id, sizeof(student.id), stdin);
            fgets(student.name, sizeof(student.name), stdin);
            fgets(student.age, sizeof(student.age), stdin);

            fputs(student.id, student_file);
            fputs(student.name, student_file);
            fputs(student.age, student_file);
            fclose(student_file);
        }
        else if (option == 3)
            return 0;
        else
        {
            printf("Invalid option\n");
            continue;
        }
    }
}
