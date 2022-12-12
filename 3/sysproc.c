#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_ok(void)
{
  return 13;
}


int
sys_change_queue(void){

  int pid, dst_level;

  if(argint(0, &pid) < 0)
    return -1;

  if(argint(1, &dst_level) < 0 || dst_level < 0 || dst_level > SCHED_LAST_QUEUE){
    return -1;
  }

  return change_queue(pid, dst_level);
}

int 
sys_assign_lottery_ticket(void){

  int pid, ticket_count;

  if(argint(0, &pid) < 0)
    return -1;

  if(argint(1, &ticket_count) < 0 || ticket_count < 0 || ticket_count > MAX_POSSIBLE_LOTTERY_TICKET){
    return -1;
  }

  return assign_lottery_ticket(pid, ticket_count);
}

int 
sys_set_bjf_params(void){

  int pid;
  int start_time, executed_cycles, priority;

  if(argint(0, &pid) < 0)
    return -1;

  if(argint(1, &start_time) < 0 || argint(2, &executed_cycles) < 0 || argint(3, &priority) < 0)
    return -1;

  return set_bjf_params(pid, start_time,executed_cycles,priority);
}

int
sys_set_sched_params(void){

  int priority_ratio;
  int start_time_ratio;
  int exec_cycle_ratio;


  if(argint(0, &priority_ratio) < 0 || argint(1, &start_time_ratio) < 0 || argint(2, &exec_cycle_ratio) < 0)
    return -1;

  

  return set_sched_params(priority_ratio,start_time_ratio,exec_cycle_ratio);
}