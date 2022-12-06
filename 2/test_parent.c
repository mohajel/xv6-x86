#include "types.h"
#include "stat.h"
#include "user.h"

void
test_parent_r(int test_levels);

int main(int argc, char *argv[])
{
    printArgLess("starting test...");

    test_parent_r(3);

    printArgLess("test finished");

    exit();
}

void
test_parent_r(int test_levels){

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

        test_parent_r(test_levels - 1);

        exit();
    }else if(child_proc > 0){
        //parent
        print("proccess %d has been forked(child id : %d)",root_pid, child_proc,NOTHING);
        wait();

        test_parent_r(test_levels - 1);
    }else{
        print("proccess %d failed on fork",root_pid, NOTHING,NOTHING);
        exit();
    }
}
