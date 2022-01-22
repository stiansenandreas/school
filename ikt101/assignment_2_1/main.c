#include <stdio.h>

int main() {
    float value = 0;
    float sum = 0;
    float count = -1;
    do {
        printf("Enter a number: \n");
        scanf("%g",&value);
        printf("You entered %g\n",value);
        sum += value;
        count += 1;
    } while (value != 0);
    float average = sum/count;
    printf("Count: %g\n",count);
    printf("Sum: %g\n",sum);
    printf("Average: %g\n",average);
    return 0;
}
