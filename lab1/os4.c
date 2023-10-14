#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK,
    OVERFLOW
} Status;

Status str_valid_check(char* str, long long* mask)
{
    char* ending;
    *mask = strtoll(str, &ending, 16);
    if (errno == ERANGE)
    {
        return OVERFLOW;
    }
    if (ending != str + strlen(str))
    {
        return NOT_OK;
    }
    return OK;
}

Status input_checker(int argc, char* argv[], long long* mask)
{
    if (argc > 4 || argc < 3)
    {
        return INVALID_INPUT;
    }
    if (argc == 4)
    {
        if (!strcmp(argv[2], "mask"))
        {
            if (str_valid_check(argv[3], mask) != OK)
            {
                return INVALID_INPUT;
            }
        }
        else
            return INVALID_INPUT;
    }
    if (argc == 3)
    {
        if (strcmp(argv[2], "xor8") && strcmp(argv[2], "xor32"))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status flag_xor8(FILE* file, unsigned int *res)
{
    unsigned char byte;
    unsigned int sum = 0;
    while (fread(&byte, sizeof(unsigned char), 1, file) == 1)
    {
        sum ^= (int)byte;
    }
    *res = sum;
    return OK;
}

Status flag_xor32(FILE* file, unsigned int *res)
{
    unsigned int sum = 0;
    unsigned char buff[4];
    int counter = 0;
    while ((counter = fread(buff, sizeof(unsigned char), 4, file)) != 0)
    {
        int number = 0;
        for (int i = 0; i < counter; i++)
        {
            number += buff[i] << (8 * (3 - i));
        }
        sum ^= number;
    }
    *res = sum;
    return OK;
}

Status flag_mask(FILE* file, long long mask, unsigned long long* res)
{
    int buff[1];
    unsigned long long counter = 0;
    while (fread(buff, sizeof(int), 1, file) == 1)
    {
        if ((~((unsigned int)mask) | (buff[0])) == UINT32_MAX) //implication
        {
            counter++;
        }
    }
    *res = counter;
    return OK;
}

int main(int argc, char* argv[])
{
    long long mask = 0;
    if (input_checker(argc, argv, &mask) != OK)
    {
        printf("Invalid input, check the entered data\n");
        return INVALID_INPUT;
    }

    FILE* file;
    if ((file = fopen(argv[1], "rb")) == NULL)
    {
        printf("File cannot be opened\n");
        return NOT_OK;
    }

    if (!strcmp(argv[2], "xor8"))
    {
        unsigned int result = 0;
        flag_xor8(file, &result);
        printf("%X\n", result);
    }
    else if (!strcmp(argv[2], "xor32"))
    {
        unsigned int result = 0;
        flag_xor32(file, &result);
        printf("%d\n", result);
    }
    else if (!strcmp(argv[2], "mask"))
    {
        unsigned long long result = 0;
        flag_mask(file, mask, &result);
        printf("%llu\n", result);
    }
    fclose(file);
    return 0;
}