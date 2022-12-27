#include "types.h"
#include "stat.h"
#include "user.h"
// #include "fcntl.h"


#define SEMCOUNT 5
#define EATROUND 3

// In the Name of God

void philosopher(int id, int right_fork, int left_fork)
{
    // for(int i = 0; i < EATROUND; i++) {
    //     if(l < r) {
    //         sem_acquire(l);
    //         sem_acquire(r);
    //     }
    //     else {
    //         sem_acquire(r);
    //         sem_acquire(l);
    //     }
    //     sleep(30 * id);
    //     printf(1, "I'm philosopher %d and I'm eating with forks %d and %d\n", id, l, r);
    //     sleep(500);
    //     sem_release(l);
    //     sem_release(r);
    //     printf(1, "I'm philosopher %d and I'm done eating\n", id);
    // }
    printf(1, "philosopher %d came to dinner\n", id);

    while (1)
    {
        printf(1, "philosopher %d decided to eat!\n", id);

        if (id % 2 == 0) //first right then left
        {
            // printf(1, "philosopher %d , acquireing right fork:%d:\n", id, right_fork);
            // sem_acquire(right_fork);
            // printf(1, "philosopher %d , acquireing left fork:%d:\n", id, left_fork);
            // sem_acquire(left_fork);
        }
        else if (id % 2 == 1) //first left then right
        {
            // printf(1, "philosopher %d , acquireing right fork:%d:\n", id, right_fork);
            // sem_acquire(right_fork);
            // printf(1, "philosopher %d , acquireing left fork:%d:\n", id, left_fork);
            // sem_acquire(left_fork);
        }
        printf(1, "philosopher %d , started eating\n", id);
        sleep(99 * id);
        sleep(500);
        // sem_release(right_fork);
        // sem_release(left_fork);
        printf(1, "philosopher %d , released both forks and started thinking\n", id);
        sleep(142 * id);
        sleep(500);
    }
}

int main(int argc, char* argv[])
{
    printf(1, "hello\n\n");
    for (int i = 0; i < 1; i++)
        sem_init(i, 1);

    for (int i = 0; i < 5; i++)
    {
        int pid = fork();
        if (pid == 0)
            philosopher(i, i, (i + 1) % 5);
    }
}
