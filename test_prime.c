#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void
test_prime(int number, int expected);

int 
call_prime_sys_call_dirty_version(int number);

int
main(int argc, char *argv[])
{
    printArgLess("starting test...");

	int i = 1;
	while(argc > i + 1){
		printArgLess("testing user entered nums : ");
		printArgLess("considered input form as num1 largest_factor1 num2 largest_factor2 ...");
		test_prime(atoi(argv[(i)]), atoi(argv[(i+1)]));
		i += 2;
	}

	printArgLess("testing predefined inputs : ");

    test_prime(726,11);
    test_prime(451,41);
    test_prime(253,23);
    test_prime(889111,43);
    test_prime(2,2);

    printArgLess("test finished");

    exit();
}

void
test_prime(int number, int expected){

    int result = call_prime_sys_call_dirty_version(number);

    print("number %d -> sys call returned %d expected %d ",number,result,expected);
    
}


int 
call_prime_sys_call_dirty_version(int number){

	int temp; //storing reg edx previous value inside temp

	__asm__("movl %%edx, %0" : "=r" (temp)); // temp = edx
	__asm__("movl %0, %%edx" : : "r"(number));// edx = number

	int result = find_largest_prime_factor();

	__asm__("movl %0, %%edx" : : "r"(temp));// edx = temp

	return result;
}
