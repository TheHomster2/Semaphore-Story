#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define KEY 1729
#define FILE "secrets"

int main(){

	int sid = semget(KEY, 1, 0644);
	  if(sid == -1){
    printf("semaphore already created\n");
    return;
	}

	printf("Please wait...Accessing resources...\n");
	struct sembuf person;
	person.sem_op = -1;
	person.sem_num = 0;
	person.sem_flag = SEM_UNDO;
	semop(sid, &person, 1);
	printf("Done!\n");

	

}
