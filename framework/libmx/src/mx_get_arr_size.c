#include "libmx.h"


int mx_get_arr_size(char **arr) {
    int counter = 0;

    while(arr[counter] != NULL) {
        counter++;
    }

    return counter;
}

