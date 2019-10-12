#pragma once

#ifndef __TINY_FILE__

#define __TINY_FILE__


#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define QUEUE_SIZE 1000
#define QUEUE_KEY 65
#define SEM_KEY 98
#define MUTEX_LOCK   0
#define MUTEX_UNLOCK   1
#define first_key "/home/shreyank/Documents/AOS/Project_2/snappy-c-master/server.c"
#define second_key "/home/shreyank/Documents/AOS/Project_2/snappy-c-master/client.c"
typedef struct 
{
    int val;
    int end_flag;
    int size_file;
    int compressed_file_size;
    pthread_mutex_t mutex;


}queue_t;


typedef struct 
{
    char file_path[1000];
    int file_path_size;
    int file_content_size;
    int uncompressed_size;
    int compressed_size;
    key_t key;
    int flag;
    char compressed_file[10000];
    queue_t queue[QUEUE_SIZE];

}stats;


typedef union  
{
    int val;
    struct semid_ds *buf;
    ushort array [1];
} semun_u;

extern stats* st_ori;
extern void queue_initialize(queue_t *);
extern void set_data(stats*);
extern stats* get_data(void);

#endif
