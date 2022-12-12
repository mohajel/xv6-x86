#include "types.h"
#include "stat.h"
#include "user.h"

void 
print(char* string){
    write(1, string, strlen(string));
}

void 
help(){
    print("set_bjf_params <pid : int> <start_time : int> <executed_cycles : int> <priority : int>");
}

int
main(int argc, char const *argv[])
{
    if(argc != 5){
        help();
        exit();
    }

    int result = set_bjf_params(atoi(argv[1]), atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));

    if(result == 0){
        print("OK");
    }else if(result == -2){
        print("ERR: Invalid PID");
    }else{
        help();
    }

    exit();
}
