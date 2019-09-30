#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
#include "snappy.h"
int main() 
{
	
	char* input_file = "Tiny.txt";
	    
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65); 

	// shmget returns an identifier in shmid 
	int shmid = shmget(key,sizeof(input_file)+1,0666|IPC_CREAT); 
	
	// shmat to attach to shared memory 
	char*str = (char*) shmat(shmid,(void*)0,0); 
	
	char*s = str;

	// for(int i=0;i<sizeof(input_file);i++){
	// 	s[i] = input_file[i];
	// }
	// s[sizeof(input_file)]=NULL;
	memcpy(str,input_file,sizeof(input_file));
	

	printf("Data written in memory: %s\n",str); 
	
	// while (*str != '*')
    //     sleep(1);


	int shmid2 = shmget(4500,3057,0666|IPC_CREAT); 
	char* compressed_file = (char*) shmat(shmid2,(void*)0,0); 
	char*out= (char*)calloc(40000, sizeof(char));
	
	char* uncompressed;
	uncompressed = (char*)calloc(40000, sizeof(char));
	while(*compressed_file!='*')
		sleep(1);
	

	memcpy(&out[0],&compressed_file[1],3100);
	snappy_uncompress(out,3100,uncompressed);
	printf("%s\n",uncompressed);
	shmdt(compressed_file);
	

	return 0; 
} 