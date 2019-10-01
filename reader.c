#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
#include "snappy.h"
#include "tiny_file.h"
int main() 
{ 
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65); 
	stats *st = (stats*)malloc(sizeof(stats));
	//stats* strin = (stats*)malloc(sizeof(stats));
	// st = get_data();
	//printf("%d",st->key);
	// shmget returns an identifier in shmid 
	
	
	int shmid = shmget(key,sizeof(stats),0666|IPC_CREAT); 

	// shmat to attach to shared memory 
	stats*strin = (stats*) shmat(shmid,NULL,0); 
	st = strin;
	printf("WHATTT %s",st->file_path);

	

FILE* fp = fopen(st->file_path, "r");
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
//st->compressed_file = (char*)malloc(sizeof(max_comp_len));
//st->compressed_file = out;
memcpy(st->compressed_file,out,max_comp_len);
size_t uncompressedLength;
snappy_uncompress(out,max_comp_len,uncompressed);
snappy_uncompressed_length(out,max_comp_len,&uncompressedLength);

printf("Size UC %ld\t C %ld\n",uncompressedLength,max_comp_len);
//printf("The file called test.dat contains this text\n\n%s", uncompressed);	
st->compressed_size = max_comp_len;
st->uncompressed_size = res;



strin = (stats*) shmat(shmid,(void*)0,0);

memcpy(strin,st,sizeof(stats));


strin->flag=1;

shmdt(strin); 
shmctl(shmid,IPC_RMID,NULL); 
 
  
  
   
  


	return 0; 
} 
