#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK
} Status;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Check the amount of entered arguments\n");
        return NOT_OK;
    }

    FILE* file_in;
    FILE* file_out;

    if ((file_in = fopen(argv[1], "rb")) == NULL)
    {
        printf("Input file cannot be opened\n");
        return NOT_OK;
    }
    if ((file_out = fopen(argv[2], "wb")) == NULL)
    {
        printf("Output file cannot be opened\n");
        fclose(file_in);
        return NOT_OK;
    }
    char ch;
    
    while (fread(&ch, sizeof(char), 1, file_in) == 1)
    {
        if (fwrite(&ch, sizeof(char), 1, file_out) != 1)
        {
            printf("Writing error occured\n");
            fclose(file_in);
            fclose(file_out);
            return NOT_OK;
        }
    }
    fclose(file_in);
    fclose(file_out);
}