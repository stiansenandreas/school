#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

int sum(const std::vector<int> &n, int const count)
{
    int sum= 0;
    for (int i= 0; i < count; i++)
        sum+= n[i];
    return sum;
}


float average(const std::vector<int> &n, int const count)
{
     return (float)sum(n,count)/(float)(count);
}


float median(const std::vector<int> &n, int const count)
{
    float median;
    if (count%2 == 0)
    {
        median= (float)(((n[(int)(floor((count-1)/2.0))]) + n[(int)(floor(count/2.0))])/2.0);
    }
    else
        median= (float)(n[(int)(floor(count/2.0))]);
    return median;
}


int main() {
    std::vector<int> numbers;
    int count= 0;

    while(true)
    {
        int input;
        std::cin >> input;
        if (input == 0)
            break;
        numbers.push_back(input);;
        count++;
    }

    std::cout << "Average : " << average(numbers,count) << std::endl;
    std::sort(numbers.begin(), numbers.end());

    std::cout << "Median : " << median(numbers,count) << std::endl;
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());

    std::cout << "Descending : ";
    for (int i= 0; i < count; i++)
        std::cout << numbers[i] << " ";

    return 0;
}
