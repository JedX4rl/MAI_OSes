#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
    INVALID_INPUT
} Status;

typedef enum
{
    MALE,
    FEMALE,
    NONE
} person;

pthread_cond_t male_condition, female_condition;
int MAX_PPL = 0;
int in_bathroom = 0;
pthread_mutex_t mutex;
person visitor = NONE;

void male_enters()
{
    pthread_mutex_lock(&mutex);
    while (in_bathroom == MAX_PPL || visitor == FEMALE)
    {
        pthread_cond_wait(&male_condition, &mutex);
    }
    visitor = MALE;
    in_bathroom++;
    printf("\tMale entered a bathroom\n");
    printf("\nBathroom is for MEN\n");
    printf("%d people in bathroom\n", in_bathroom);
    pthread_mutex_unlock(&mutex);
}

void female_enters()
{
    pthread_mutex_lock(&mutex);
    while (in_bathroom == MAX_PPL || visitor == MALE)
    {
        pthread_cond_wait(&female_condition, &mutex);
    }
    visitor = FEMALE;
    in_bathroom++;
    printf("\tFemale entered a bathroom\n");
    printf("\nBathroom is for WOMEN\n");
    printf("%d people in bathroom\n", in_bathroom);
    pthread_mutex_unlock(&mutex);
}

void male_leaves()
{
    pthread_mutex_lock(&mutex);
    in_bathroom--;
    printf("One man leaves the bathroom\n");
    if (in_bathroom == 0)
    {
        visitor = NONE;
        printf("The bathroom is free\n");
        pthread_cond_broadcast(&female_condition);
    }
    printf("%d people in bathroom\n", in_bathroom);
    pthread_mutex_unlock(&mutex);
}

void female_leaves()
{
    pthread_mutex_lock(&mutex);
    in_bathroom--;
    if (in_bathroom == 0)
    {
        visitor = NONE;
        printf("The bathroom is free\n");
        pthread_cond_broadcast(&male_condition);
    }
    printf("There are %d people in bathroom\n", in_bathroom);
    pthread_mutex_unlock(&mutex);
}

void* thread_m(void* x)
{
    int male_cnt = 1;
    for (int i = 0; i < male_cnt; ++i)
    {
        usleep((rand() %9 + 1) * 100000);
        male_enters();
        usleep(500000);
        male_leaves();
    }
    return NULL;
}

void* thread_f(void *x)
{
    int female_cnt = 1;
    for (int i = 0; i < female_cnt; ++i)
    {
        usleep((rand() %9 + 1) * 100000);
        female_enters();
        usleep(1000000);
        female_leaves();
    }
    return NULL;
}

void solution(int male_cnt, int female_cnt)
{
    pthread_t male[male_cnt], female[female_cnt];
    for (int i = 0; i < male_cnt; ++i)
    {
        pthread_create(&male[i], NULL, thread_m, NULL);
    }
    for (int i = 0; i < female_cnt; ++i)
    {
        pthread_create(&female[i], NULL, thread_f, NULL);
    }
    for (int i = 0; i < male_cnt; ++i)
    {
        pthread_join(male[i], NULL);
    }

    for (int i = 0; i < female_cnt; ++i)
    {
        pthread_join(female[i], NULL);
    }
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid input, enter a Number\n");
        return INVALID_INPUT;
    }
    char* temp;
    MAX_PPL = strtol(argv[1], &temp, 10);
    unsigned male_cnt, female_cnt;

    printf("Enter number of male persons:\n");
    scanf("%d", &male_cnt);

    printf("Enter number of female persons:\n");
    scanf("%d", &female_cnt);

    if (male_cnt > MAX_PPL || female_cnt > MAX_PPL)
    {
        printf("Invalid input, total person_cnt must be equal to or less than MAX_PPL\n");
        return  INVALID_INPUT;
    }
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&male_condition, NULL);
    pthread_cond_init(&female_condition, NULL);

    solution((int)male_cnt, (int)female_cnt);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&male_condition);
    pthread_cond_destroy(&female_condition);

    return 0;
}
