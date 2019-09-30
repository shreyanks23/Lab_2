#ifndef __TINY_FILE__

#define __TINY_FILE__

#define QUEUE_SIZE 1000
typedef struct tiny_file
{
    int val;
}queue_t;


typedef struct 
{
    char*file_path;
    int uncompressed_size;
    int compressed_size;
    queue_t queue[QUEUE_SIZE];

}stats;


void queue_initialize(queue_t *);

#endif
