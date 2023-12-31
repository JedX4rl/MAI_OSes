#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void Usage()
{
    printf("Program for making some magic with file\n");
    printf("Enter filename\n");
}
typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK
} Status;

int main(int argc, char* argv[])
{   
    Usage();
    if (argc != 2)
    {
        printf("Check the amount of entered arguments\n");
        return INVALID_INPUT;
    }

    FILE* file;
    if ((file = fopen(argv[1], "w")) == NULL)
    {
        printf ("The file cannot be opened\n");
        return NOT_OK;
    }
    const int byte_cnt = 11;
    const int byte_cnt_2 = 4;
    char in[byte_cnt] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    if (fwrite(in, sizeof(char), byte_cnt, file) != byte_cnt)
    {
        printf("Writing error occured\n");
        fclose(file);
        return NOT_OK;
    }
    fclose(file);

    if ((file = fopen(argv[1], "r")) == NULL)
    {
        printf ("The file cannot be opened\n");
        return NOT_OK;
    }

    char ch;

    while (fread(&ch, sizeof(ch), 1, file))
    {
        printf("%x %d %d %d %p\n", ch, file->_r, file->_w, file->_flags, file->_p);
    }
    fclose(file);

     if ((file = fopen(argv[1], "r")) == NULL)
     {
        printf ("The file cannot be opened\n");
        return NOT_OK;
     }
     fseek(file, 3, SEEK_SET);
     char out[byte_cnt_2];
     if (fread(out, sizeof(char), byte_cnt_2,file) != byte_cnt_2)
     {
        printf("Error occured\n");
        fclose(file);
        return NOT_OK;
     }

     for (int i = 0; i < byte_cnt_2; i++)
     {
        printf("%x ", out[i]);
     }
     printf("\n");
     fclose(file);

    return 0;
}