#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
#include "snappy.h"
#include "tiny_file.h"
 key_t key;
stats* st_ori;
void queue_initialize(queue_t *q){
    q->val=0;
}

void set_data(stats *st){
    st_ori = (stats*)malloc(sizeof(stats));
    st_ori->key = st->key;
    
    for(int i=0;i<QUEUE_SIZE;i++)
    {    st_ori->queue[i].val = st->queue[i].val;
       // printf("%d",st_ori.queue[i].val);
    }
}

stats* get_data(){
    //st_ori = (stats*)malloc(sizeof(stats));
    printf("Key in function is %d",st_ori->queue[0].val);
    return st_ori;
}


