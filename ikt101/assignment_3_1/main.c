#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define STR_LEN 20



bool is_palindrome(char const word[], int len)
{
    char first_half[STR_LEN+1];
    char second_half[STR_LEN+1];
    int middle_index;
    if (len%2 == 0)
    {
        middle_index= len/2;
        for (int i=0; i < middle_index; i++)
            first_half[i]= word[i];
        for (int i=0; i < middle_index; i++)
            second_half[i]= word[len-i-1];
    }
    if (len%2 != 0)
    {
        middle_index= floor(len/2.0);
        for (int i=0; i < middle_index; i++)
            first_half[i]= word[i];
        for (int i=0; i < middle_index; i++)
            second_half[i]= word[len-i-1];
    }
    if (strcmp(first_half,second_half) == 0)
        return true;
    else
        return false;

}

void string_reverse(char word[], int len)
{
    char copied[STR_LEN+1];
    strcpy(copied, word);
    for (int i= 0; i < len; i++)
    {
        word[i]= copied[len-i-1];
    }
}

int main() {
    printf("Enter a word\n");
    char word[STR_LEN+1];
    scanf("%s", word);
    int len= (int)strlen(word);
    printf("The word contains %d letters\n", len);
    bool palindrome= is_palindrome(word, len);
    if (palindrome == true)
        printf("The word is a palindrome\n");
    else
        printf("The word is not a palindrome\n");
    string_reverse(word,len);
    printf("The word reversed is '%s'\n", word);
    return 0;
}
