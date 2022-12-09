#include "types.h"
#include "stat.h"
#include "user.h"

void 
print(char* string){
    write(1, string, strlen(string));
}

void 
help(){
    print("change_queue <pid : int> <dst_level : {0 (RR), 1 (Lottery), 2 (BJF)}>");
}

int
main(int argc, char const *argv[])
{
    if(argc != 3){
        help();
        exit();
    }

    int result = change_queue(atoi(argv[1]), atoi(argv[2]));

    if(result == 0){
        print("OK");
    }else if(result == -2){
        print("ERR: Invalid PID");
    }else{
        help();
    }

    exit();
}
