#include <stdio.h>
#include <string.h>
#define STR_LEN 60


int main() {
    printf("Enter two strings!\n");
    char string_1[STR_LEN+1]= {0};
    char string_2[STR_LEN+1]= {0};
    scanf("%s %s", string_1, string_2);

    int eval= strcmp(string_1, string_2);
    if (eval < 0 || eval > 0)
        printf("The words are not equal\n");
    else
        printf("The words are equal\n");

    char *is_substring_1= strstr(string_1,string_2);
    char *is_substring_2= strstr(string_2,string_1);

    if (is_substring_1 != NULL)
        printf("Word 2 is a substring of word 1");
    if (is_substring_2 != NULL)
        printf("Word 1 is a substring of word 2");
    if (is_substring_1 == NULL && is_substring_2 == NULL)
        printf("No substrings found");

    return 0;
}
