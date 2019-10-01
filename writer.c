#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
#include "snappy.h"
#include "tiny_file.h"
int main() 
{
	
	char* input_file = "Tiny.txt";
	    
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65);
	stats*st = (stats*)malloc(sizeof(stats));
	//stats*st_2 = (stats*)malloc(sizeof(stats));
	st->key = key;
	printf("Key is %d",key);
	for(int i=0;i<QUEUE_SIZE;i++)
	{
		st->queue[i].val=0;
	}

	st->file_path_size = sizeof(input_file);
	st->flag=0;

	memcpy(st->file_path,input_file,sizeof(input_file));

	int shmid = shmget(key,sizeof(stats),0666|IPC_CREAT); 
	
	// shmat to attach to shared memory 
	stats*st_2 = (stats*) shmat(shmid,NULL,0); 
	
	
	memcpy(st_2,st,sizeof(stats)); 
	

	printf("File name written in memory: %s\n",st_2->file_path); 
	printf("Struct size is %ld\n",sizeof(stats));
	
	st_2 = (stats*) shmat(shmid,NULL,0); 
	st = st_2;
	
	char* uncompressed;
	uncompressed = (char*)calloc(st->uncompressed_size, sizeof(char));
	while(st_2->flag==0)
		sleep(1);
	
	
	//char*out= (char*)calloc(st->uncompressed_size, sizeof(char));
	//memcpy(&out[0],&compressed_file[1],st->compressed_size);
	snappy_uncompress(st->compressed_file,st->compressed_size,uncompressed);
	printf("Uncompressed Size is\t%s\n",uncompressed);
	shmdt(st_2);
	

	return 0; 
} 