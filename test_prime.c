#include "user.h"
#include "fcntl.h"

#define DEFAULT_NUM 276

int main(int argc, char* argv[])
{
	printf(1, "........test prime......\n");

	int number = DEFAULT_NUM;
	int temp; //storing reg edx previous value inside temp

	if (argc == 2)
		number = atoi(argv[1]);
	

	__asm__("movl %%edx, %0" : "=r" (temp)); // temp = edx
	__asm__("movl %0, %%edx" : : "r"(number));// edx = number

	int result = find_largest_prime_factor();

	__asm__("movl %0, %%edx" : : "r"(temp));// edx = temp

	printf(1, "largest_prime_factor of %d is %d\n", number, result);
	exit();
}
