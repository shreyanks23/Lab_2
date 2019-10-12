#include <sys/ipc.h> 
#include <sys/shm.h> 
#include<sys/msg.h>
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
#include "snappy.h"
#include "tiny_file.h"
#include <pthread.h>




int main() 
{ 

	printf("here");
	int sms_size=64;
	int n_sms = 5;
	key_t *key_arr  = (key_t*)malloc(10*sizeof(key_t));
	int *shmid_arr = (int*)malloc(10*sizeof(int));
	char *input_char = (char*) malloc(sms_size);
	key_t msg_key = ftok(first_key,65);
	key_t msg_key2 = ftok(second_key,65);
	int msgid;
	if((msgid = msgget(10, 0666|IPC_CREAT))==-1){
		perror("msgget error");
	}
	int msgid2 = msgget(25, 0666|IPC_CREAT);
	queue_t* queue_shared = (queue_t*)malloc(sizeof(queue_t));

	for(int i=0;i<n_sms;i++){
		key_arr[i] = i;
		shmid_arr[i] = shmget(key_arr[i], sms_size, 0666|IPC_CREAT);
	}

	queue_initialize(queue_shared);
	queue_shared->val = 1;
	
	printf("%d",msgid);
	if((msgsnd(msgid,queue_shared,sizeof(queue_t),0))==-1 )
	{
		perror("msgsnd error");
		exit(1);
	}
	queue_shared->val=0;
	msgrcv(msgid2,queue_shared,sizeof(queue_t),0,0);

	msgctl(msgid, IPC_RMID, NULL); 
	msgctl(msgid2, IPC_RMID, NULL);

	msgid = msgget(10, 0666|IPC_CREAT);
	msgid2 = msgget(25, 0666 | IPC_CREAT); 
	queue_shared->val=1;
	



//////////////////////////////////////////////////////////////
	char *input_file = (char*) malloc(queue_shared->size_file);
	printf("END FLAG HERE IS %d",queue_shared->end_flag);
	
	while (queue_shared->end_flag==0)
	{
		queue_shared->val=1;
		for(int i=0;i<n_sms;i++){
			msgrcv(msgid,queue_shared,sizeof(queue_t),0,0);
			input_char =  shmat(shmid_arr[i],NULL,0);
			strcat(input_file,input_char);
			msgsnd(msgid2,queue_shared,sizeof(queue_t),0);
			
		}
	}

printf("%s",input_file);
	
shmdt(input_char);
for(int i=0;i<n_sms;i++)
	shmctl(shmid_arr[i],IPC_RMID,NULL);

char*buffer;
char*uncompressed;
size_t res = queue_shared->size_file;
struct snappy_env en;
snappy_init_env(&en);
if(snappy_init_env(&en)==0)
{
   printf("Success\n");
}
size_t max_comp_len = snappy_max_compressed_length(res);
char *out = (char*)calloc(max_comp_len, sizeof(char));

snappy_compress(&en,input_file, res, out, &max_comp_len);
size_t uncompressedLength;
snappy_uncompressed_length(out,max_comp_len,&uncompressedLength);

printf("\nSize UC %ld\t C %ld\n",uncompressedLength,max_comp_len);
queue_shared->end_flag=0;
queue_shared->val=1;
queue_shared->compressed_file_size = max_comp_len;

msgsnd(msgid,queue_shared,sizeof(queue_t),0);



msgctl(msgid, IPC_RMID, NULL); 
msgctl(msgid2, IPC_RMID, NULL);

msgid = msgget(10, 0666|IPC_CREAT);
msgid2 = msgget(25, 0666 | IPC_CREAT); 
queue_shared->val=1;

int track=0;
int count=0;

	while(count<=max_comp_len){
		queue_shared->end_flag=0;
		printf("%d\t%d\n",count,max_comp_len);
		for(int i=0;i<n_sms;i++)
		{
			input_char =  shmat(shmid_arr[i],NULL,0);
			memcpy(input_char,&out[track*sms_size],sms_size);
			track++;
			if(track>=(max_comp_len/sms_size))
				{
					count=max_comp_len;
					break;
				}
			if(msgsnd(msgid,queue_shared,sizeof(queue_t),0)==-1){
				perror("server sending");
			}
			msgrcv(msgid2,queue_shared,sizeof(queue_t),0,0);
		}
		count+=sms_size;
		queue_shared->val=1;
	}

	queue_shared->end_flag=1;
	msgsnd(msgid,queue_shared,sizeof(queue_t),0);

//////////////////////////////////////////////////////////////////////
	return 0; 
} 
