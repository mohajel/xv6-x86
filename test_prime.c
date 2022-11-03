#include "user.h"
#include <stdio.h>
#include <string.h>
#define NOTHING -1

void 
print(char* message, int first_arg, int second_arg, int third_arg);

void
test_prime(int number, int expected);

int
main(int argc, char *argv[])
{
    print("starting test...",NOTHING,NOTHING,NOTHING);

    test_prime(726,23);
    test_prime(451,41);
    test_prime(253,23);
    test_prime(889111,43);
    test_prime(2,2);

    print("test finished",NOTHING,NOTHING,NOTHING);

    exit();
}

void 
print(char* message, int first_arg, int second_arg, int third_arg){

    char buffer[250] = {0};

    sprintf(buffer, message, first_arg, second_arg, third_arg);

    strcat(buffer, "\n");

    write(0,buffer, strlen(buffer));
}

void
test_prime(int number, int expected){

    int result = find_largest_prime_factor(number);

    print("number %d -> sys call returned %d expected %d",number,result,expected);
    
}
