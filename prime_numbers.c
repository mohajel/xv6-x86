// in the nae of God

// my first user program sample which uses user defined syscals
#include "types.h"
#include "stat.h"
#include "user.h"
#include <stdbool.h>

int string_to_int(char* str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return res;
}    

bool is_prime(int num)
{
  if ((num == 1) | (num == 2) | (num == 3))
    return true;
  for (int i = 2; i <= num / 2; ++i)
    if (num % i == 0)
      return false;

  return true;
}
    
int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf(1, "........invalid input......\n");
    exit();
  }
  int first_num = string_to_int(argv[1]);
  int second_num = string_to_int(argv[2]);
  int smaller_num = (first_num < second_num) ? first_num : second_num;
  int bigger_num = (first_num < second_num) ? second_num : first_num;

  for (int i = smaller_num; i <= bigger_num; ++i)
    if (is_prime(i))
      printf(2, "  %d,  ", i);

  exit();
}
