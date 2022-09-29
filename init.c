// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };


//FUNC
void 
print_group_names()
{
  printf(1, " \t --- ------------------------------- --- \n");
  printf(1, " \t --- In the Name of God              --- \n");
  printf(1, " \t --- M.sadegi -- 810199483           --- \n");
  printf(1, " \t --- M.backshayesh -- 810199381      --- \n");
  printf(1, " \t --- ------------------------------- --- \n");
}

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf(1, "init: starting sh\n");

    print_group_names();
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
