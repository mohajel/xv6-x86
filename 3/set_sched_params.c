#include "types.h"
#include "stat.h"
#include "user.h"

void 
print(char* string){
    write(1, string, strlen(string));
}

void 
help(){
    print("set_sched_params <priority_ratio : int> <start_time_ratio : int> <exec_cycle_ratio : int>");
}

int
main(int argc, char const *argv[])
{
    if(argc != 4){
        help();
        exit();
    }

    int result = set_sched_params(atoi(argv[1]), atoi(argv[2]),atoi(argv[3]));

    if(result == 0){
        print("OK");
    }else if(result == -2){
        print("ERR: Invalid PID");
    }else{
        help();
    }

    exit();
}
