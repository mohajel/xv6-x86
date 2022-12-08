// In the Name of God

// my first user program sample which uses user defined syscals
#include "types.h"
#include "stat.h"
#include "user.h"
    
 //passing command line arguments
    
int main(int argc, char *argv[])
{
  printf(1, "........sample 1......\n");
  printf(2, "my favorite number is : %d\n", ok());

  // int rand;
  // int x = (int) &rand;
  // printf(1, "%d", x);

  int id = fork();

  if (id == 0)
  {
    // fork();
    fork();
    while(1)
    {
      printf(1, "0\n");
      // sleep(200);
    }
  }
  else
  {
    while(1)
    {
      printf(1, "1\n");
      // sleep(200);
    }

  }
  
  
  exit();
}

