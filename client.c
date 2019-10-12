#include <sys/ipc.h> 
#include <sys/shm.h> 
#include<sys/msg.h>
#include <stdio.h> 
 #include <stdlib.h>
 #include<string.h>
 #include<sys/sem.h>
 #include <pthread.h>
#include "snappy.h"
#include "tiny_file.h"

void client_call(queue_t * queue_shared, int res, int n_sms, int sms_size, char*input_char, int* shmid_arr, char* file_content, int msgid, int msgid2)

{
	queue_shared->val=1;
	int track=0;
	int count=0;
	while(count<=res){

		for(int i=0;i<n_sms;i++)
		{
			input_char =  shmat(shmid_arr[i],NULL,0);
			memcpy(input_char,&file_content[track*sms_size],sms_size);
			printf("%d\t%s\n",track,input_char);	
			track++;
			if(track>=(res/sms_size))
				{
					count=res;
					break;
				}
			if(msgsnd(msgid,queue_shared,sizeof(queue_t),0)==-1){
				perror("Client sending");
			}
			msgrcv(msgid2,queue_shared,sizeof(queue_t),0,0);
		}
		count+=sms_size;
		queue_shared->val=1;
	}

	queue_shared->end_flag=1;
	msgsnd(msgid,queue_shared,sizeof(queue_t),0);
	msgrcv(msgid,queue_shared,sizeof(queue_t),0,0);
	
	msgctl(msgid, IPC_RMID, NULL); 
	msgctl(msgid2, IPC_RMID, NULL);

	msgid = msgget(10, 0666|IPC_CREAT);
	msgid2 = msgget(25, 0666 | IPC_CREAT); 
	queue_shared->val=1;

	char * output_file = (char*)malloc(queue_shared->compressed_file_size);

	while (queue_shared->end_flag==0)
	{	

		queue_shared->val=1;
		
		for(int i=0;i<n_sms;i++){
			msgrcv(msgid,queue_shared,sizeof(queue_t),0,0);
			input_char =  shmat(shmid_arr[i],NULL,0);
			strcat(output_file,input_char);
			if(msgsnd(msgid2,queue_shared,sizeof(queue_t),0)==-1)
			{
				perror("error here");
			}
			if(queue_shared->end_flag==1){
				break;
			}	
		}
	}
}

int main() 
{
	
	int sms_size = 64;
	int n_sms=5;
	key_t *key_arr  = (key_t*)malloc(10*sizeof(key_t));
	int *shmid_arr = (int*)malloc(10*sizeof(int));
	char *input_char = (char*) malloc(sms_size);
	key_t msg_key = ftok(first_key,65);
	key_t msg_key2 = ftok(second_key,65);
	int msgid = msgget(10, 0666|IPC_CREAT);
	int msgid2 = msgget(25, 0666|IPC_CREAT);
	queue_t* queue_shared = (queue_t*)malloc(sizeof(queue_t));

	for(int i=0;i<n_sms;i++)
	{

		key_arr[i] = i;
		shmid_arr[i] = shmget(key_arr[i], sms_size, 0666|IPC_CREAT);
	}

	char* input_file = "Tiny.txt";
	FILE* fp = fopen(input_file, "r");
	fseek(fp, 0L, SEEK_END);
	size_t res = ftell(fp);
	fseek(fp,0L,SEEK_SET);


	char* file_content = (char*)malloc(res);
	fread(file_content, sizeof(char), res, fp);
	fclose(fp);

	int count=0;
	int n;

	msgrcv(msgid,queue_shared,sizeof(queue_t),0,0);

	queue_shared->size_file = res;
	queue_shared->val=1;
	
	msgsnd(msgid2,queue_shared,sizeof(queue_t),0);


	msgctl(msgid, IPC_RMID, NULL); 
	msgctl(msgid2, IPC_RMID, NULL);

	msgid = msgget(10, 0666|IPC_CREAT);
	msgid2 = msgget(25, 0666 | IPC_CREAT); 
	

///////////////////////////////////////////////////////////////////////////
	client_call(queue_shared,res,n_sms, sms_size, input_char, shmid_arr, file_content,msgid, msgid2);
////////////////////////////////////////////////////////////////////////////
	return 0; 
} 