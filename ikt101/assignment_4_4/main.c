#include <stdio.h>
#include <string.h>
#define STR_LEN 60

int main() {
    printf("Enter a word\n");
    char word[STR_LEN+1];
    scanf("%s", word);

    int str_len = (int)strlen(word);
    char alphabet[26]="abcdefghijklmnopqrstuvwxyz";
    int letter_count[26]={0};

    for (int i= 0; i < str_len; i++)
    {
        for (int j=0; j < 26; j++)
        {
            if (word[i] == alphabet[j])
            {
                letter_count[j]++;
                printf("'%c' : %d\n", alphabet[j],letter_count[j]);
            }
        }
    }

    return 0;
}
