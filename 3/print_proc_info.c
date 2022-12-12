#include "types.h"
#include "stat.h"
#include "user.h"

void 
print(char* string){
    write(1, string, strlen(string));
}

int
main(int argc, char const *argv[])
{   
    print_proc_info();
    
    exit();
}
