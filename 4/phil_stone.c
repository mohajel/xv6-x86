// in the name of God

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void philosopher(int id, int right_fork, int left_fork)
{
    while (1)
    {
        printf(1, "P:%d decided to eat!\n", id);

        if (id % 2 == 0) //first right then left
        {
            sem_acquire(right_fork);
            // printf(1, "P:%d a qr right\n", id, right_fork);

            sem_acquire(left_fork);
            // printf(1, "P:%d aqr left\n", id, left_fork);
        }
        else if (id % 2 == 1) //first left then right
        {
            sem_acquire(left_fork);
            // printf(1, "P:%d aqr left\n", id, left_fork);

            sem_acquire(right_fork);
            // printf(1, "P:%d aqr right\n", id, right_fork);
        }
        printf(1, "P:%dstarted eating\n", id);
        sleep(9 * id);
        sleep(50);
        sem_release(right_fork);
        sem_release(left_fork);
        printf(1, "P:%d released forks\n", id);
        sleep(12 * id);
        sleep(50);
    }
}
 

int main(int argc, char* argv[])
{
    printf(1, "once upon a time, in hogwarts...\n\n");
    for (int i = 0; i < 5; i++)
        sem_init(i, 1);

    for (int i = 0; i < 5; i++)
    {
        int pid = fork();
        if (pid == 0)
            philosopher(i, i, (i + 1) % 5);
    }
    exit();
}
