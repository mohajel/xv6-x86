#include "user.h"
#include "syscall.h"
#include <stdio.h>
#include <string.h>
#define NOTHING -1

int print(char* message, int arg);

int wait_wrapper(void);
int write_wrapper(void*);
int fork_wrapper(void);

int
main(int argc, char *argv[])
{
    write_wrapper("starting test...");

    write_wrapper("testing write...");

    write_wrapper("testing write again!...");
    
    int pid = fork_wrapper();

    if(pid == 0){
        write_wrapper("testing write from child!!...");

        int new_pid = fork_wrapper();

        if(new_pid == 0){
            get_callers(SYS_fork);

            write_wrapper("testing write from child!!...");

            //no exit! this child proc will continue to run!
        }else{
            get_callers(SYS_wait);
            wait_wrapper();
            get_callers(SYS_wait);
        }

        new_pid = fork_wrapper();
        if(new_pid == 0){
            exit();
        }else{
            wait_wrapper();
        }

        exit();
    }else{
        write_wrapper("testing write before wait!!...");
        wait_wrapper();

        write_wrapper("testing write after wait!!...");
        
    }

    write_wrapper("test finished");

    get_callers(SYS_write);
    get_callers(SYS_fork);
    get_callers(SYS_wait);

    exit();
}

int wait_wrapper(void){
    print("TEST INFO -- wait called by pid : %d", getpid());
    return wait();
}
int fork_wrapper(void){
    print("TEST INFO -- fork called by pid : %d", getpid());
    int child = fork();

    if(child > 0)
        print("\tchild pid is %d", child);

    return child;

}

int write_wrapper(void* text){
    print("TEST INFO -- write called by pid : %d", getpid());
    return print(text, NOTHING);
}

int print(char* message, int arg){
    char buffer[250] = {0};

    sprintf(buffer, message, arg);

    strcat(buffer, "\n");

    return write(0,buffer, strlen(buffer));
}
