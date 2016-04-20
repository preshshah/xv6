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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
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

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}

int
sys_clone(void)
{
	int func;
	int arg;
	int stack;

	if(argint(0,&func) < -1){
      return -1;
  }

  if(argint(1,&arg) < 0) {
    return -1;
  }

  if(argint(2,&stack) < 0){
    return -1;
  } 

	clone((void *) func, (void *) arg, (void *) stack);
}

int
sys_join(void)
{
	int pid;
	int stack;
	int retval;

	if(argint(0,&pid) < -1){
      return -1;
  }

  if(argint(1,&stack) < 0) {
    return -1;
  }

  if(argint(2,&retval) < 0){
    return -1;
  } 
	
	join(pid, (void **) stack, (void **) retval);
}

int
sys_texit(void)
{
	int retval;
	
	if(argint(0,&retval) < -1){
      return -1;
  }

  proc->retval = (void *) retval;
  exit();
}

int
sys_mutex_init(void)
{
	return mutex_init();
}

int
sys_mutex_destroy(void)
{
	int mutex_id;

	if(argint(0,&mutex_id) < -1){
      return -1;
  }

	if(mutex_id < 0 || mutex_id > 31){
		//cprintf("Mutex ID must be between 0 and 31\n");
		return -1;
	}

	if(*(proc->mutex_table[mutex_id].flag) == 1){
		///cprintf("Trying to destory a locked mutex\n");
		return -1;
	}

	if(*(proc->mutex_table[mutex_id].mystate) == 0){
		//cprintf("This mutex is uninitialized or already destroyed\n");
		return -1;
	}
	
	return mutex_destroy(mutex_id);
}

int
sys_mutex_lock(void)
{
	int mutex_id;

	if(argint(0,&mutex_id) < -1){
      		return -1;
  	}

	if(mutex_id < 0 || mutex_id > 31){
		//cprintf("Mutex ID must be between 0 and 31\n");
		return -1;
	}

	if(*(proc->mutex_table[mutex_id].mystate) == 0){
		//cprintf("This lock is uninitialized or destoryed\n");
		return -1;
	}
	
	return mutex_lock(mutex_id);
}

int
sys_mutex_unlock(void)
{
	int mutex_id;

	if(argint(0,&mutex_id) < -1){
      		return -1;
  	}

	if(mutex_id < 0 || mutex_id > 31){
		//cprintf("Mutex ID must be between 0 and 31\n");
		return -1;
	}

	if(*(proc->mutex_table[mutex_id].mystate) == 0){
		//cprintf("This lock is uninitialized or destoryed\n");
		return -1;
	}

	return mutex_unlock(mutex_id);
}




