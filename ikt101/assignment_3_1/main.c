#include <stdio.h>
#include <math.h>
#define ARRAY_SIZE 10


int minimum(int const numbers[], int const size)
{
    int min= numbers[0];
    for(int i= 1; i < size; i++)
    {
        if(min > numbers[i])
            min= numbers[i];
    }
    return min;
}


int maximum(int const numbers[], int const size)
{
    int max= numbers[0];
    for(int i= 1; i < size; i++)
    {
        if(max < numbers[i])
            max= numbers[i];
    }
    return max;
}


int sum (int const numbers[], int const size)
{
    int sum= numbers[0];
    for (int i= 1; i < size; i++)
        sum+= numbers[i];
    return sum;
}


float average (int numbers[], int size)
{
    float average= (float)sum(numbers,size)/(float)size;
    return average;
}


void sort (int numbers[], int const size)
{
    for (int i= 0; i < size; i++)
    {
        for (int j= 0; j < size-i-1; j++)
        {
            if (numbers[j] > numbers[j+1])
            {
                int swap_numbers= numbers[j];
                numbers[j]= numbers[j+1];
                numbers[j+1]= swap_numbers;
            }
        }
    }
}


float median(int const numbers[], int size)
{
    float median;
    if (size%2 == 0)
        median= (float)((numbers[(int)floor((size-1)/2.0)] + numbers[(int)floor(size/2.0)])/2.0);
    else
        median= (float)numbers[(int)floor(size/2.0)];
    return median;
}



int main()
{
    printf("Enter 10 numbers: \n");
    int inputArray[ARRAY_SIZE];
    for(int i= 0; i < ARRAY_SIZE; i++)
    {
        scanf("%d", &inputArray[i]);
        printf("You entered %d\n", inputArray[i]);
    }
    printf("Minimum: %d\n",minimum(inputArray,ARRAY_SIZE));
    printf("Maximum: %d\n",maximum(inputArray,ARRAY_SIZE));
    printf("Sum: %d\n",sum(inputArray,ARRAY_SIZE));
    printf("Average: %g\n",average(inputArray,ARRAY_SIZE));
    printf("Sorted: ");
    sort(inputArray,ARRAY_SIZE);
    for (int i= 0; i< ARRAY_SIZE; i++)
        printf("%d ",inputArray[i]);
    printf("\nMedian: %g",median(inputArray,ARRAY_SIZE));
    return 0;
}
