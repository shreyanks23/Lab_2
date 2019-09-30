#include "snappy.h"
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
int main() 
{ 
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65); 

	// shmget returns an identifier in shmid 
	int shmid = shmget(key,8,0666|IPC_CREAT); 

	// shmat to attach to shared memory 
	char *strin = (char*) shmat(shmid,(void*)0,0); 


  char*in = strin;
  

FILE* fp = fopen(in, "r");
fseek(fp, 0L, SEEK_END);
size_t res = ftell(fp);
fseek(fp,0L,SEEK_SET);
printf("size %ld\n",res); 


char*buffer;
char*uncompressed;
buffer = (char*)calloc(res, sizeof(char));	
char*out = (char*)calloc(res, sizeof(char));	
uncompressed = (char*)calloc(res, sizeof(char));
fread(buffer, sizeof(char), res, fp);
fclose(fp);
struct snappy_env en;
snappy_init_env(&en);
if(snappy_init_env(&en)==0)
{
   printf("Success\n");
}
size_t max_comp_len = snappy_max_compressed_length(sizeof(buffer));


snappy_compress(&en,buffer, res, out, &max_comp_len);
size_t uncompressedLength;
snappy_uncompress(out,max_comp_len,uncompressed);
snappy_uncompressed_length(out,max_comp_len,&uncompressedLength);

printf("Size UC %ld\t C %ld\n",uncompressedLength,max_comp_len);
//printf("The file called test.dat contains this text\n\n%s", uncompressed);	


// memcpy(strin,out,uncompressedLength);
  
	//detach from shared memory 
	shmdt(strin); 
	
	// destroy the shared memory 
	shmctl(shmid,IPC_RMID,NULL); 


  // key_t key2 = ftok("shmfile",65); 

	// // shmget returns an identifier in shmid 
	 int shmid2 = shmget(4500,max_comp_len,0666|IPC_CREAT); 

	// // shmat to attach to shared memory 
	 char *strin2 = (char*) shmat(shmid2,(void*)0,0);

   memcpy(strin2,out,max_comp_len);

    *strin2 = '*';

   shmdt(strin2); 
   shmctl(shmid2,IPC_RMID,NULL); 
 
  
  
   
  


	return 0; 
} 
