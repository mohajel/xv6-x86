#include "types.h"
#include "stat.h"
#include "user.h"

typedef void (*Proc_Main)(void);

void 
time_consuming_method(){
    
    volatile double a = 0.0505551515,b = 55666.56464,c = 66454656.666;

    for(int i = 0; i < __INT_MAX__/10; i++){
        a = 5*b + 9*c + 3*a + 1;
        a++;
    }

    b = a;
}

void 
run_proc(Proc_Main main){
    if(fork() == 0){
        printf(1,"child proc %d has been started\n", getpid());
        main();
        exit();
    }
}


int
main(int argc, char const *argv[])
{


    int children_count = 10;

    printf(1,"proc %d is running\n", getpid());

    for(int i = 0; i < children_count; i++){
        run_proc(time_consuming_method);
    }

    int pid;
    
    while ((pid = wait()) != -1){
        printf(1,"child proc (%d) ended -- %d childs left\n",pid,--children_count);
    }
    
    printf(1,"proc %d is exiting\n", getpid());

    exit();
}
