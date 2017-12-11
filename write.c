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
	int shmd = shmget(KEY, 128 * sizeof(char), 0);

	  if(sid == -1){
    printf("semaphore already created\n");
    return 1;
	}

	printf("Please wait...Accessing resources...\n");
	struct sembuf person;
	person.sem_op = -1;
	person.sem_num = 0;
	person.sem_flg = SEM_UNDO;
	semop(sid, &person, 1);
	printf("Done!\n");

	  if(shmd == -1){
    printf("shared memory already created\n");
    return 1;
  }

  	int *size = shmat(shmd, 0, 0);

	int fd = open(FILE, O_CREAT | O_TRUNC, 0644);
	lseek(fd, *size * -1, SEEK_END);
	char lastLine[*size];
	read(fd, lastLine, *size);

	printf("Last line of the story: %s\n", lastLine);
	printf("Put in your line:\n");
	char input[256];
	fgets(input, sizeof(input), stdin);
	*size = strlen(input);
  	write(fd, input, *size);
  	shmdt(size);
  	person.sem_op = 1;
	semop(sid, &person, 1);

	return 0;
}
