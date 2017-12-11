#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#define KEY 1729
#define FILE "secrets"

int main(){

	int sid = semget(KEY, 1, 0644);
	int shmd = shmget(KEY, 128 * sizeof(char), 0);

  if(sid == -1){
    printf("semaphore already created\n");
    return 1;
	}

	printf("Please wait...Accessing resources...\n");
	struct sembuf person;
	person.sem_num = 0;
	person.sem_op = -1;
	person.sem_flg = SEM_UNDO;
	semop(sid, &person, 1);
	printf("Done!\n");

  if(shmd == -1){
    printf("shared memory already created\n");
    return 1;
  }

	// shared memory stores last line
  char *line = (char *) shmat(shmd, 0, 0);
	// perror("shmat");
	int fd = open(FILE, O_CREAT | O_TRUNC, 0644);
	// get new line
	printf("Last line of the story: %s\n", line);
	printf("Put in your line:");
	fgets(line, 128, stdin);
	// do stuff
	write(fd, line, 128);
	shmdt(line);
	person.sem_op = 1;
	semop(sid, &person, 1);
}
