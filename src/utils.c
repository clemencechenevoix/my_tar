// ----- include ----- //
#include "../include/utils.h"

// ----- function ----- //

/** 
* @brief count the number of characters in a string
* @param char* str (a word or a sentence)
* @return int (the length of the string)
*/
int my_length(char* string) {
    int index = 0;
    while (string[index] != '\0') {
        index += 1;
    }
    return index;
}

int my_power(int number, int power) {
    if(power == 0) {
        return 1;
    }

    int index = 1;
    int result = number;
    while(index < power) {
        result = result * number;
        index += 1;
    }
    return result;
}

/** 
* @brief copy string_copy into string_var
* @param char* string_var (a malloc buffer with enough place)
* @param char* string_copy (the string to copy)
* @return void
*/
void my_strcpy(char* string_var, char* string_copy) {
    int index = 0;
    int length = my_length(string_copy);
    while (index < length) {
        string_var[index] = string_copy[index];
        index += 1;
    }
    string_var[index] = '\0';
    return;
}

/** 
* @brief replace all character of the given string by a given character
* @param char* string
* @param char charact
* @param int length (the length of the string)
* @return void
*/
void my_memset(char* string, char charact, int length) {
    int index = 0;
    while (index < length) {
        string[index] = charact;
        index += 1;
    }
}

int my_write(char* string, int fd) {
    return write(fd, string, my_length(string));
}

void my_itoa(char* buffer, int number) {
    int index_buffer = 0;
    int index_number = 0;
    int dividend = number;
    int remainder;

    while (dividend != 0) {
        dividend = dividend / 10;
        index_number += 1;
    }

    dividend = number;
    while (index_number > 0) {
        buffer[index_buffer] = 48 + (number / my_power(10,(index_number-1))) % 10;
        index_buffer += 1;
        index_number -= 1;
    }
    buffer[index_buffer] = '\0';
};

int my_atoi(char* string) {
    return 0;
};