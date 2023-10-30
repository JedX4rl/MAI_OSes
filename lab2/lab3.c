#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define PHL_CNT 5

pthread_t philosophers[PHL_CNT];
pthread_mutex_t forks[PHL_CNT];

void* philosopher(void *ID)
{
    int philosopher_ID = *((int*) ID);
    int left_fork = philosopher_ID;
    int right_fork = (philosopher_ID + 1) % PHL_CNT;

    while (true)
    {
        printf("Philosopher %d is thinking...\n", philosopher_ID);
        usleep(1000000);

        pthread_mutex_lock(&forks[left_fork]);
        printf("Philosopher %d took a fork %d\n", philosopher_ID, left_fork);
        usleep(500000);

        pthread_mutex_lock(&forks[right_fork]);
        printf("Philosopher %d took a fork %d\n", philosopher_ID, right_fork);

        printf("Philosopher %d is eating\n", philosopher_ID);
        usleep(1000000);

        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);

        printf("Philosopher %d put fork %d\n", philosopher_ID, left_fork);
        printf("Philosopher %d put fork %d\n", philosopher_ID, right_fork);

    }
    return NULL;
}

int main()
{
    int i;
    int philosopher_IDs[PHL_CNT];

    // init mutex for forks
    for (i = 0; i < PHL_CNT; ++i)
    {
        pthread_mutex_init(&forks[i], NULL);
    }
    //create philosopher threads
    for (i = 0; i < PHL_CNT; i++)
    {
        philosopher_IDs[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void*)&philosopher_IDs[i]);
    }
    //wait till threads finish
    for (i = 0; i < PHL_CNT; ++i)
    {
        pthread_join(philosophers[i], NULL);
    }
    //destroy mutex
    for (i = 0; i < PHL_CNT; ++i)
    {
        pthread_mutex_destroy(&forks[i]);
    }
    return 0;
}