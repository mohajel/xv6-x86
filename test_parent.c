#include "user.h"
#include <stdio.h>
#include <string.h>
#define NOTHING -1

void 
print(char* message, int first_arg, int second_arg, int third_arg);

void
test_parent(int test_levels);

int
main(int argc, char *argv[])
{
    print("starting test...",NOTHING,NOTHING,NOTHING);

    test_parent(3);

    print("test finished",NOTHING,NOTHING,NOTHING);

    exit();
}



void
test_parent(int test_levels){

    if(test_levels <= 0){
        return;
    }

    int root_pid = getpid();

    int child_proc = fork();
    if(child_proc == 0){
        //child
        print("TEST : current pid : %d, parent : %d, get_parent_pid : %d",getpid(), root_pid, get_parent_pid());
        if(root_pid != get_parent_pid()){
            print("\tFail! test level %d", test_levels,NOTHING,NOTHING);
        }else{
            print("\tsuccess! test level %d", test_levels,NOTHING,NOTHING);
        }

        test_parent(test_levels - 1);

        exit();
    }else if(child_proc > 0){
        //parent
        print("proccess %d has been forked(child id : %d)",root_pid, child_proc,NOTHING);
        wait();

        test_parent(test_levels - 1);
    }else{
        print("proccess %d failed on fork",root_pid, NOTHING,NOTHING);
        exit();
    }
}


void 
print(char* message, int first_arg, int second_arg, int third_arg){

    char buffer[250] = {0};

    sprintf(buffer, message, first_arg, second_arg, third_arg);

    strcat(buffer, "\n");

    write(0,buffer, strlen(buffer));
}
