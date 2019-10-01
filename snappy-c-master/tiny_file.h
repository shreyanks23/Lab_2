#pragma once

#ifndef __TINY_FILE__

#define __TINY_FILE__


#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define QUEUE_SIZE 1000
typedef struct 
{
    int val;
}queue_t;


typedef struct 
{
    char file_path[1000];
    int file_path_size;
    int uncompressed_size;
    int compressed_size;
    key_t key;
    int flag;
    queue_t queue[QUEUE_SIZE];

}stats;

extern stats* st_ori;
extern void queue_initialize(queue_t *);
extern void set_data(stats*);
extern stats* get_data(void);

#endif
