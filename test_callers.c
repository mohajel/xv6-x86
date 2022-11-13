#include "types.h"
#include "user.h"
#include "syscall.h"

int wait_wrapper(void);
int write_wrapper(void*);
int fork_wrapper(void);

void print_info()
{
    printf(1, " ---- PID is %d: \n ", getpid());
    printf(1, " ---- calling  SYS_write: \n ");
    get_callers(SYS_write);
    printf(1, " ---- calling  SYS_fork: \n ");
    get_callers(SYS_fork);
    printf(1, " ---- calling  SYS_wait: \n ");
    get_callers(SYS_wait);   
    printf(1, " ---- ---- ---- ---- ---- ---- ---- \n\n ");
}

void test_2()
{
    printf(1, " ---- test_2 started \n ");
    print_info();

    int x = fork();

    if (x == 0) // child processs
    {
        printf(1, " ---- calling inside child process\n ");
        print_info();
    }
    else
        wait();  
}


void tast_1()
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

    get_callers(SYS_write);
    get_callers(SYS_fork);
    get_callers(SYS_wait);

    write_wrapper("test finished");
}


int
main(int argc, char *argv[])
{
    // test_1();

    test_2();

    exit();

}

int wait_wrapper(void){
    printSingleArg("TEST INFO -- wait called by pid : %d", getpid());
    return wait();
}
int fork_wrapper(void){
    printSingleArg("TEST INFO -- fork called by pid : %d", getpid());
    int child = fork();

    if(child > 0)
        printSingleArg("\tchild pid is %d", child);

    return child;

}

int write_wrapper(void* text){
    printSingleArg("TEST INFO -- write called by pid : %d", getpid());
    return printSingleArg(text, NOTHING);
}

