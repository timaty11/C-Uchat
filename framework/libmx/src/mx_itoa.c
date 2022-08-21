#include "libmx.h"

char *mx_itoa(int number)
{
    if (number == 0)
    {
        return mx_strdup("0");
    }
    int copy = number;
    int count = 0;
    char curr = '\0';
    if (number < 0)
        count++;
    while (copy != 0)
    {
        count++;
        copy /= 10;
    }
    char *res = mx_strnew(count);
    if (number < 0)
        res[0] = '-';
    while (number != 0)
    {
        curr = number % 10;
        number /= 10;
        curr = curr > 0 ? curr : -curr;
        res[count - 1] = curr + '0';
        count--;
    }
    return res;
}
