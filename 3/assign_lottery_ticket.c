#include "types.h"
#include "stat.h"
#include "user.h"

void 
print(char* string){
    write(1, string, strlen(string));
}

void 
help(){
    print("assign_lottery_ticket <pid : int> <ticket count : int>");
}

int
main(int argc, char const *argv[])
{
    if(argc != 3){
        help();
        exit();
    }

    int result = assign_lottery_ticket(atoi(argv[1]), atoi(argv[2]));

    if(result == 0){
        print("OK");
    }else if(result == -2){
        print("ERR: Invalid PID");
    }else{
        help();
    }

    exit();
}
