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

/*
	class header in control.c
*/
int main(){
	// get semaphore
	int sid = semget(KEY, 1, 0644);

	// check for error
  if(sid == -1){
    printf("run ./control first\n");
    return 1;
	}

	// semaphore things
	printf("Please wait...Accessing resources...\n");
	struct sembuf person;
	person.sem_num = 0;
	person.sem_op = -1;
	person.sem_flg = SEM_UNDO;
	semop(sid, &person, 1);
	printf("Done!\n");

	// get last line
	int shmd = shmget(KEY, sizeof(int), 0);
  int *size = (int *) shmat(shmd, 0, 0);
	// printf("%d\n", *size);
	// perror("shmat");
	int fd = open(FILE, O_RDONLY, 0644);
  lseek(fd, -1 * *size, SEEK_END);
	char *line = calloc (128, sizeof(char));
	read(fd, line, *size);
	// perror("read");
	close(fd);

	// get new line
	printf("Last line of the story: %s\n", line);
	printf("Put in your line: ");
	char input[128];
	fgets(input, 128, stdin);
	// perror("fgets");
	// printf("%sAAAAA\n", input);
	free(line);

	// do stuff with the things
	*size = strlen(input);
	int fd2 = open(FILE, O_WRONLY | O_APPEND);
	// perror("open");
	write(fd2, input, strlen(input));
	// perror("write");
	person.sem_op = 1;
	semop(sid, &person, 1);
	close(fd2);
}
