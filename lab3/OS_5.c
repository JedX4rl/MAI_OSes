#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_SIZE 512

typedef enum
{
    OK,
    INVALID_ARGUMENT,
    OPENING_ERROR,
    DIRECTORY_ERROR,
    NOT_OK
} Status;

void Errors(Status st)
{
    switch (st)
    {
        case OK:
            return;
        case INVALID_ARGUMENT:
            printf("Invalid argument\n");
            return;
        case OPENING_ERROR:
            printf("File opening error\n");
            return;
        case DIRECTORY_ERROR:
            printf("Directory error detected\n");
            return;
        case NOT_OK:
            printf("Something went wrong\n");
            return;
        default:
            printf("Something went wrong\n");
    }
}

Status print_to_file(const char* path, const char* filename)
{
    if (path == NULL || filename == NULL)
    {
        return INVALID_ARGUMENT;
    }
    struct stat buff;
    int check = stat(path, &buff);
    if (check == -1)
    {
        return OPENING_ERROR;
    }
    printf("\t%s", filename);
    if (S_ISDIR(buff.st_mode))
    {
        printf("[directory]\n");
    }
    else if (S_ISREG(buff.st_mode))
    {
        printf("[file]\n");
    }
    else if (S_ISLNK(buff.st_mode))
    {
        printf("[symlink]\n");
    }
    else if (S_ISFIFO(buff.st_mode))
    {
        printf("[FIFO]\n");
    }
    else if (S_ISSOCK(buff.st_mode))
    {
        printf("[socket]\n");
    }
    else if (S_ISCHR(buff.st_mode))
    {
        printf("[character device]\n");
    }
    else if (S_ISBLK(buff.st_mode))
    {
        printf("[block device]\n");
    }
    else
    {
        printf("[unknown]\n");
    }
    return OK;
}

Status my_ls(const char* path)
{
    if (path == NULL)
    {
        return INVALID_ARGUMENT;
    }
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL)
    {
        return DIRECTORY_ERROR;
    }
    printf("%s\n", path);
    while ((entry = readdir(dir)) != NULL)
    {
        char f_path[MAX_SIZE];
        struct stat statistic;
        snprintf(f_path, sizeof(f_path), "%s/%s", path, entry->d_name);
        Status st = print_to_file(f_path, entry->d_name);
        if (st)
        {
            closedir(dir);
            return NOT_OK;
        }
    }
    closedir(dir);
    return OK;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        Errors(INVALID_ARGUMENT);
        return INVALID_ARGUMENT;
    }
    for (int i = 1; i < argc; ++i)
    {
        Status st = my_ls(argv[i]);
        if (st)
        {
            Errors(st);
            return st;
        }
    }
    return 0;
}
