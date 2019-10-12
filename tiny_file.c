#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
 #include <pthread.h>
#include "snappy.h"
#include "tiny_file.h"



void queue_initialize(queue_t *q){
    q->val=0;
    q->end_flag=0;
    q->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    
}


