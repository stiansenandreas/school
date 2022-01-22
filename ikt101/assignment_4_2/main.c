#include <stdio.h>
#include <string.h>
#include <math.h>
#define STR_LEN 60


void convert_to_lowercase(char *string)
{
    for (int i=0; string[i] != '\0'; i++)
    {
        if (string[i] >= 'A' && string[i] <= 'Z')
        {
            string[i]= string[i] +32;
        }
    }
}


void convert_to_uppercase(char *string)
{
    for (int i=0; string[i] != '\0'; i++)
    {
        if (string[i] >= 'a' && string[i] <= 'z')
        {
            string[i]= string[i] -32;
        }
    }
}


void split_string(char const string[])
{
    int str_len= (int)strlen(string);
    char first_half[STR_LEN+1]={0};
    char second_half[STR_LEN+1]={0};
    int middle_index;
    if (str_len%2 == 0)
    {
        middle_index= str_len/2;
        for (int i= 0; i < middle_index; i++)
        {
            first_half[i]= string[i];
            second_half[i]= string[i+middle_index];
        }
    }
    if (str_len%2 != 0)
    {
        middle_index= floor(str_len/2.0);
        for (int i= 0; i < middle_index; i++)
        {
            first_half[i]= string[i];
        }
        for (int i=0; i < middle_index+1; i++)
            second_half[i]= string[i+middle_index];
    }
    printf("The word split in two is '%s - %s'", first_half,second_half);
}


int main()
{
    char string[STR_LEN+1]={0};
    char exp_string[STR_LEN+1]={0};
    printf("Enter a string\n");
    scanf("%s", string);
    strcpy(exp_string,string);
    convert_to_lowercase(exp_string);
    printf("The word in lowercase is '%s'\n", exp_string);
    convert_to_uppercase(exp_string);
    printf("The word in uppercase is '%s'\n", exp_string);
    split_string(string);
    return 0;
}
