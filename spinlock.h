#ifndef SPINLOCK_H
#define SPINLOCK_H

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?
  
  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};

struct mymutex{
	int *flag;
	int *mystate;
	struct spinlock lock;
	void *chan;
};
typedef struct mymutex mymutex;

//0 = mutex uninitialized/destroyed, 1 = mutex initialized
//0 = unlocked, 1 = locked

#endif
