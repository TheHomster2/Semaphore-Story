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
#include <stdlib.h>

#define KEY 1729
#define FILE "secrets"

int main(){

	int sid = semget(KEY, 1, 0644);
	int shmd = shmget(KEY, sizeof(int), 0);

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

	// shared memory stores size of last line
  int *size = (int *) shmat(shmd, 0, 0);
	printf("%d\n", *size);
	// perror("shmat");
	int fd = open(FILE, O_RDWR, 0644);
  lseek(fd, -1 * *size, SEEK_END);
	char *line = calloc (128, sizeof(char));
	read(fd, line, *size);
	close(fd);
	// get new line
	printf("Last line of the story: %s\n", line);
	printf("Put in your line: ");
	char *input;
	fgets(input, 128, stdin);
	printf("%s\n", input);
	// do stuff
	*size = strlen(input);
	fd = open(FILE, O_APPEND);
	write(fd, input, 128);
	shmdt(line);
	person.sem_op = 1;
	semop(sid, &person, 1);
}
