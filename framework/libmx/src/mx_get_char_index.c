#include "libmx.h"

int mx_get_char_index(const char *str, char c)
{
    if (str == NULL) return -2;
    int s_len = mx_strlen(str);
    for (int i = 0; i <= s_len; i++)
    {
        if (str[i] == c) return i;
    }
    return -1;
}
