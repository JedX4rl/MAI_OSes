#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC,
    NOT_OK,
    FILE_OPEN_ERROR,
    NULL_POINTER_ERROR,
    CREATING_PROCESS_ERROR
} Status;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NOT_OK:
            printf("Not OK\n");
            return;
        case FILE_OPEN_ERROR:
            printf("File opening error\n");
            return;
        case NULL_POINTER_ERROR:
            printf("NULL pointer error\n");
            return;
        case CREATING_PROCESS_ERROR:
            printf("Creating process error\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status get_str(FILE* file, char** str, int* buff)
{
    if (str == NULL)
    {
        return NULL_POINTER_ERROR;
    }
    *buff = 256;
    *str = (char*)malloc(sizeof(char) * (*buff));
    if (*str == NULL)
    {
        return BAD_ALLOC;
    }
    int temp = 0;
    int c = fgetc(file);
    while (c != EOF && c != '\n')
    {
        if (temp >= (*buff) - 1)
        {
            char* temp_str = (char*)realloc(*str, ((*buff) *= 2));
            if (temp_str == NULL)
            {
                free(str);
                return BAD_ALLOC;
            }
            (*str) = temp_str;
        }
        (*str)[temp++] = c;
        c = fgetc(file);
    }
    (*str)[temp] = '\0';
    return OK;
}

Status is_sub_string(FILE** file, char* str, int* found)
{
    int str_len = (int)strlen(str);
    int position = 0;
    int num_of_str = 1;
    int nothing = 1;
    int temp = 0;
    int flag_eof = 0, flag_n = 0;
    char c;

    while ((c = fgetc(*file)) != EOF && flag_eof == 0)
    {
        if (flag_n == 1)
        {
            position = 0;
            num_of_str++;
        }
        temp = 0;
        flag_n = 0;
        if (c == '\n')
        {
            flag_n = 1;
        }
        while (c == str[temp] && temp < str_len)
        {
            c = fgetc(*file);
            if (c == EOF)
            {
                flag_eof = 1;
            }
            temp++;
        }
        if (temp == str_len)
        {
            nothing = 0;
        }
        if (temp > 1)
        {
            fseek(*file, -temp, SEEK_CUR);
            position++;
        }
        else if (temp == 1)
        {
            fseek(*file, -1, SEEK_CUR);
        }
        position++;
    }
    if (nothing == 0)
    {
        (*found) = 1;
        return OK;
    }
    return NOT_OK;
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }

    FILE* file_path;

    if ((file_path = fopen(argv[1], "r")) == NULL)
    {
        errors(FILE_OPEN_ERROR);
        return FILE_OPEN_ERROR;
    }

    int buff_size = 0;
    char* filename = NULL;
    int *found = mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    while(!feof(file_path))
    {
        if (get_str(file_path, &filename, &buff_size) == BAD_ALLOC)
        {
            errors(BAD_ALLOC);
            free(filename);
            fclose(file_path);
            return BAD_ALLOC;
        }
        if (filename == NULL)
        {
            printf("da\n");
            return FILE_OPEN_ERROR;
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            errors(CREATING_PROCESS_ERROR);
            return CREATING_PROCESS_ERROR;
        }
        else if (pid == 0)
        {
            FILE* curr_file;
            if ((curr_file = fopen(filename, "r")) == NULL)
            {
                errors(FILE_OPEN_ERROR);
                free(filename);
                fclose(file_path);
                return FILE_OPEN_ERROR;
            }
            Status status = is_sub_string(&curr_file, argv[2], found);
            if (status == OK)
            {
                printf("String found in file %s\n", filename);
            }
            fclose(curr_file);
            exit(0);
        }
    }

    int status;
    while (wait(&status) > 0);

    if (*found == 0)
    {
        printf("The string was not found in any file\n");
    }

    fclose(file_path);
    free(filename);
    munmap(found, sizeof(int));
    return 0;
}
