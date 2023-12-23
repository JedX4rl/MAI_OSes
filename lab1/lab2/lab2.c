#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include "/Users/nikitatretakov/Uni/Labs/MathPracticum/Laboratory_work4/lab6/my_lib.h"
#include <time.h>

typedef struct user
{
    char* login;
    char* pin;
    int status;
    struct user* next;
    char banned;
} user;

typedef struct List
{
    user* head;
    user* tail;
    ull size;
} List;

__attribute__((unused)) Status null_list(List* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

Status create_list(List** list)
{
    if (list == NULL)
    {
        return INVALID_ARGUMENT;
    }
    (*list)->head = (user*)malloc(sizeof(user));
    if ((*list)->head == NULL)
    {
        return BAD_ALLOC;
    }
    (*list)->head = (*list)->tail = NULL;
    (*list)->size = 0;
    return OK;
}

Status destruct_list(List* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    if (list->head == NULL || list->tail == NULL)
    {
        return list->head == list->tail ? OK : BAD_ALLOC;
    }
    user* item = list->head;
    user* next_item = item->next;
    while (item->next != NULL)
    {
        free(item);
        item = next_item;
        next_item = item->next;
    }
    free(item);
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

void destruct_user(user* User)
{
    if (User == NULL)
    {
        return;
    }
    free(User->login);
    free(User->pin);
    free(User);
}

Status push_back_list(List* users, user* User)
{
    if (users == NULL || User == NULL)
    {
        return INVALID_INPUT;
    }
    if (users->tail != NULL)
    {
        users->tail->next = User;
    }
    if (users->head == NULL)
    {
        users->head = User;
    }
    users->tail = User;
    User->next = NULL;
    users->size++;
    return OK;
}


user* find_user(List* users, const char* login)
{
    if (users == NULL || login == NULL)
    {
        return NULL;
    }
    user* current = users->head;
    while (current != NULL)
    {
        if (!strcmp(current->login, login))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
Status user_exists(List* users, user* new_user)
{
    if (users == NULL)
    {
        return INVALID_ARGUMENT;
    }
    user* current = users->head;
    while (current != NULL)
    {
        if (!strcmp(current->login, new_user->login))
        {
            return EXISTS;
        }
        current = current->next;
    }
    return OK;
}

__attribute__((unused)) Status push_front_list(List* users, user * User)
{
    if (users == NULL || User == NULL)
    {
        return INVALID_INPUT;
    }
    if (users->head != NULL)
    {
        User->next = users->head;
    }
    if (users->tail == NULL)
    {
        users->tail = User;
    }
    users->head = User;
    users->size++;
    return OK;
}

Status add_user(List* users, user* User)
{
    if (users == NULL || User == NULL)
    {
        return INVALID_INPUT;
    }
    if (users->size == 0)
    {
        push_back_list(users, User);
        return OK;
    }
    user* current = users->head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    if (current->next == NULL)
    {
        push_back_list(users, User);
        return OK;
    }
    User->next = current->next;
    current->next = User;
    return OK;
}

__attribute__((unused)) void print_list(List list)
{
    user* item = list.head;
    if (list.size == 0)
    {
        printf("List is empty\n");
        return;
    }
    while (item != NULL)
    {
        printf("%s %d\n", item->login, item->status);
        item = item->next;
    }
    printf("\n");
}

Status login_is_valid(const char* login)
{
    if (login == NULL)
    {
        return INVALID_ARGUMENT;
    }
    int length = (int) strlen(login);
    if (length > 6)
    {
        return INVALID_ARGUMENT;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!isalnum(login[i]))
        {
            return INVALID_ARGUMENT;
        }
    }
    return OK;
}

Status pin_is_valid(const char* pin)
{
    if (pin == NULL)
    {
        return INVALID_ARGUMENT;
    }
    for (int i = 0; i < pin[i]; ++i)
    {
        if (!isnumber(pin[i]))
        {
            return INVALID_INPUT;
        }
    }
    errno = 0;
    char* ptr;
    ll res = strtoll(pin ,&ptr, 10);
    if (*ptr != '\0')
    {
        return INVALID_ARGUMENT;
    }
    if (errno == ERANGE)
    {
        return OVERFLOWED;
    }
    if (res > 100000)
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status registration(List* users)
{
    if (users == NULL)
    {
        return INVALID_ARGUMENT;
    }
    user* User = (user*)malloc(sizeof(user));
    if (User == NULL)
    {
        return BAD_ALLOC;
    }
    char* login = NULL;
    char* password = NULL;
    printf("Enter login\n");
    Status st = get_str(&login);
    st = st ? st : login_is_valid(login);
    if (!st)
    {
        printf("Enter pin\n");
    }
    st = st ? st : get_str(&password);
    st = st ? st : pin_is_valid(password);
    if (!st)
    {
        User->login = login;
        User->pin = password;
        User->banned = 'N';
        User->status = 0;
        st = user_exists(users, User);
        if (st == EXISTS)
        {
            free(login);
            free(password);
            return EXISTS;
        }
        st = st ? st : add_user(users, User);
    }
    if (st)
    {
        free(login);
        free(password);
        free(User);
        return st;
    }
    return OK;
}

Status log_in(List* Users)
{
    if (Users == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (Users->size == 0)
    {
        printf("You are new user! Sign up, please!\n");
        return NEW_USER;
    }
    char* login = NULL;
    char* password = NULL;
    printf("Enter login\n");
    Status st = get_str(&login);
    st = st ? st : login_is_valid(login);
    if (!st)
    {
        printf("Enter pin\n");
    }
    st = st ? st : get_str(&password);
    st = st ? st : pin_is_valid(password);
    if (!st)
    {
        user* found;
        found = find_user(Users, login);
        if (found)
        {
            if (!strcmp(password, found->pin))
            {
                free(login);
                free(password);
                if (found->banned == 'Y')
                {
                    found->banned = 'N';
                }
                printf("You're in!\n");
                return LOGGED;
            }
            else
            {
                st = INCORRECT_PASSWORD;
                printf("Wrong password!\n");
                free(password);
                free(login);
                return st;
            }
        }
        else
        {
            printf("You are new user! Sign up, please!\n");
            st = NEW_USER;
            free(password);
            free(login);
            return st;
        }
    }
    return st;
}

Status sanctions(List* Users, int* amount_of_commands)
{
    if (Users == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (Users->size == 0)
    {
        printf("The list of users is empty\n");
        return INVALID_INPUT;
    }
    char* login = NULL;
    printf("Enter login\n");
    Status st = get_str(&login);
    st = st ? st : login_is_valid(login);
    if (!st)
    {
        user* found = find_user(Users, login);
        if (found)
        {
            printf("Are you sure? Print 12345 to continue\n");
            char* tmp = NULL;
            st = get_str(&tmp);
            if (st)
            {
                free(login);
                free(tmp);
                return st;
            }
            if (strcmp(tmp, "12345") != 0)
            {
                printf("Cancelled\n");
                free(login);
                free(tmp);
                return OK;
            }
            if (found->banned == 'Y')
            {
                printf("User is already banned!\n");
                free(login);
                return OK;
            }
            found->banned = 'Y';
            printf("Enter the number of available commands\n");
            char* number = NULL;
            st = get_str(&number);
            if (!st)
            {
                *amount_of_commands = strtol(number, NULL, 10);
            }
            free(number);
            free(login);
            return BANNED;
        }
        else
        {
            free(login);
            printf("Unknown login\n");
            st = UNKNOWN_LOGIN;
            return st;
        }
    }
    free(login);
    return st;
}

void print_time()
{
    time_t curr_time;
    struct tm *local_time;
    curr_time = time(NULL);
    local_time = localtime(&curr_time);
    char buff[256];
    strftime(buff, 256, "%H:%M:%S\n", local_time);
    fputs(buff, stdout);
}

void print_date()
{
    time_t curr_time;
    struct tm *local_time;
    curr_time = time(NULL);
    local_time = localtime(&curr_time);
    char buff[256];
    strftime(buff, 256, "%d:%m:%Y\n", local_time);
    fputs(buff, stdout);
}

Status string_to_time(const char* str_time, time_t* time)
{
    if (str_time == NULL || str_time[10] != '\0' || str_time[2] != ':' || str_time[5] != ':')
    {
        return INVALID_INPUT;
    }
    for (int i = 0; i < 10; ++i)
    {
        if (str_time[i] == '\0' || (i != 2 && i != 5 && !isdigit(str_time[i])))
        {
            return INVALID_INPUT;
        }
    }

    char str_day[11], str_month[11], str_year[11];
    strcpy(str_day, str_time);
    strcpy(str_month, str_time + 3);
    strcpy(str_year, str_time + 6);
    str_day[2] = str_month[2] = str_year[4] = '\0';

    int day = atoi(str_day);
    int month = atoi(str_month);
    int year = atoi(str_year);

    char month_content[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    char leap_month_content[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (year <= 1970 || year > 3000 || month > 12)
    {
        return INVALID_INPUT;
    }

    if (!(year % 400 == 0 && year % 100 != 0 && year % 4))
    {
        if (day > leap_month_content[month-1])
        {
            return INVALID_INPUT;
        }
    }
    else
    {
        if (day > month_content[month-1])
        {
            return INVALID_INPUT;
        }
    }

    struct tm tmp;
    tmp.tm_sec = tmp.tm_min = tmp.tm_hour = 0;
    tmp.tm_isdst = -1;
    tmp.tm_mday = day;
    tmp.tm_mon = month - 1;
    tmp.tm_year = year - 1900;

    *time = mktime(&tmp);
    return OK;
}

Status how_much(const char* string, const char* flag)
{
    if (string == NULL || flag == 0)
    {
        return INVALID_INPUT;
    }
    ll difference = 0;
    time_t cur_time = time(NULL);
    time_t some_time;
    if (string_to_time(string, &some_time))
    {
        return INVALID_INPUT;
    }
    else if (*flag != '-' || flag[2] != '\0')
    {
        return INVALID_ARGUMENT;
    }
    else
    {
        switch (flag[1])
        {
            case 's':
                difference = cur_time - some_time;
                printf("%lld seconds passed\n", difference);
                break;
            case 'm':
                difference = (cur_time - some_time) / 60;
                printf("%lld minutes passed\n", difference);
                break;
            case 'h':
                difference = (cur_time - some_time) / 3600;
                printf("%lld hours passed\n", difference);
                break;
            case 'y':
                difference= (cur_time - some_time) / 3600 / 24 / 365;
                printf("%lld years passed\n", difference);
                break;
            default:
                return INVALID_INPUT;
        }
    }
    return OK;
}


int main()
{
    int running = 1;
    Status st = OK;
    List* Users;
    st = create_list(&Users);
    while (running && !st)
    {
        char* command = NULL;
        printf("Hello! Enter: <sign in> to login to account or <exit> to exit\n");
        st = get_str(&command);
        int logged = 0;
        int banned = 0;
        int amount_of_commands = 0;
        if (!strcmp(command, "sign in"))
        {
            st = log_in(Users);
            st = (st == NEW_USER || st == INCORRECT_PASSWORD || st == UNKNOWN_LOGIN) ? registration(Users) : st;
            if (st == EXISTS)
            {
                st = OK;
            }
            logged = st == LOGGED || st == OK ? 1 : 0;
            if (st == LOGGED)
            {
                st = OK;
            }
        }
        else if (!strcmp(command, "exit"))
        {
            running = 0;
        }
        else
        {
            printf("Unknown command!\n");
        }
        while (logged && !st)
        {
            if (banned)
            {
                printf("You have %d actions left\n", amount_of_commands);
            }
            if (banned && !amount_of_commands)
            {
                printf("You have no rules! Logged out to continue\n");
                logged = 0;
            }
            else
            {
                printf("Available commands: Time, Date, How much, Logout, Sanctions\n");
                char *curr_command = NULL;
                st = get_str(&curr_command);
                if (!st)
                {
                    if (!strcmp(curr_command, "Time"))
                    {
                        if (banned)
                        {
                            amount_of_commands--;
                        }
                        print_time();
                    }
                    else if (!strcmp(curr_command, "Date"))
                    {
                        if (banned)
                        {
                            amount_of_commands--;
                        }
                        print_date();
                    }
                    else if (!strcmp(curr_command, "How much"))
                    {
                        if (banned)
                        {
                            amount_of_commands--;
                        }
                        char *time = NULL;
                        char *flag = NULL;
                        printf("Enter date: dd:mm:yyyy\n");
                        st = get_str(&time);
                        if (!st)
                        {
                            printf("Enter flag: -s (seconds) -m (minutes) -h (hours) -y (years)\n");
                        }
                        st = st ? st : get_str(&flag);
                        st = st ? st : how_much(time, flag);
                        free(time);
                        free(flag);
                    }
                    else if (!strcmp(curr_command, "Sanctions"))
                    {
                        st = sanctions(Users, &amount_of_commands);
                        if (st == UNKNOWN_LOGIN)
                        {
                            st = OK;
                        }
                        if (st == BANNED)
                        {
                            st = OK;
                            banned = 1;
                            amount_of_commands--;
                        }
                    }
                    else if (!strcmp(curr_command, "Logout"))
                    {
                        logged = 0;
                    }
                    else
                    {
                        printf("Unknown command!\n");
                    }
                    free(curr_command);
                }
            }

        }
        free(command);
    }
    if (st)
    {
        errors(st);
    }
    destruct_list(Users);
    return st;
}
