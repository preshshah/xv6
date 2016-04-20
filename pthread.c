#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

// Implement your pthreads library here.

int pthread_create(pthread_t* thread, const pthread_attr_t *attr, void *(*start_routine) (void*), void *arg){
	thread->stack = (void*) malloc(4096);
	thread->tflag = 1;
	thread->func = (void *)(start_routine);
	thread->arg = arg;
	thread->pid = clone(thread->func,thread->arg,thread->stack);
	return thread->pid;
	
}

int pthread_join(pthread_t thread, void** retval){
	int join_return = join(thread.pid, (void **)thread.stack, retval);
	free(thread.stack);
	return join_return;
}

int pthread_exit(void* retval){
	texit(retval);
	return 0;
}

int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t * attr){
	mutex->mutex_id = mutex_init();
	//printf(1,"Mutex ID: %d\n",mutex->mutex_id);
	return mutex->mutex_id;
}
int pthread_mutex_destroy(pthread_mutex_t * mutex){
	return mutex_destroy(mutex->mutex_id);
}
int pthread_mutex_lock(pthread_mutex_t * mutex){
	return mutex_lock(mutex->mutex_id);
}
int pthread_mutex_unlock(pthread_mutex_t * mutex){
	return mutex_unlock(mutex->mutex_id);
}

