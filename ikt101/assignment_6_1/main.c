#include <stdio.h>
#include <stdlib.h>

int main() {
    int count, size;

    printf("Enter array size\n");
    scanf("%d", &size);
    if (size == 0)
    {
        printf("No numbers were given");
        return 1;
    }

    int *numbers = malloc(size*sizeof(int));
    
    printf("Enter %d numbers\n", size);
    for (count= 0; count < size; count++)
    {
        scanf("%d", &numbers[count]);
    }

    printf("Count: %d\nNumbers:", count);
    for (int i= 0; i < count; i++)
        printf(" %d", numbers[i]);

    printf("\nSorted:");
    for (int i=0; i < count; i++)
    {
        for (int j= 0; j < count-i-1; j++)
        {
            if (numbers[j] > numbers[j+1])
            {
                int swap= numbers[j];
                numbers[j]= numbers[j+1];
                numbers[j+1]= swap;
            }
        }
    }

    for (int i=0; i < count; i++)
        printf(" %d", numbers[i]);

    free(numbers);
    return 0;
}
