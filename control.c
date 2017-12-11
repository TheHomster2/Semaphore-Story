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
#include <errno.h>

#define KEY 1729
#define FILE "secrets"

/*
  Anthony Hom Charles Weng
  Systems pd 4
  HW 15 - Semaphone
  2017 - 12 - 11

  below code is based on charles' repo

  i figured it out after taking another look, when i was shmgetting the memory in rem(), i did indeed put in bad arguments and the size of the memory was wrong
*/

// helper for create flag; creates the semaphore if not already created
void create() {
  int sid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);

  // check for error
  if(sid == -1){
    printf("semaphore already created\n");
    return;
  }

  semctl(sid, 0, SETVAL, 1);
  // perror("semctl");
  printf("semaphore created: %d\n", sid);
  int shmd = shmget(KEY, 128 * sizeof(char), IPC_CREAT | IPC_EXCL | 0644);
  printf("shared memory created: %d\n", shmd);
  printf("value set: %d\n", semctl(sid, 0, SETVAL, 1));

  int fd = open(FILE, O_CREAT | O_TRUNC | O_EXCL, 0644);
  close(fd);
  printf("file created:%d\n", fd);
}

// helper for view flag; returns
void view() {
  if (semget(KEY, 1, 0)){
    printf("initialize first\n");
    return;
  }

  char buf[128];
  printf("The Story:\n");
  // FILE * doesn't seem to work as an initializer and stderr isn't used
  stderr = fopen(FILE, "r");
  // loop through and print out each line
  while(fgets(buf, sizeof(buf), stderr))
    printf("\t%s", buf);
}

// helper for remove flag
void rem() {
  int sem_desc, mem_desc;
  // printf("Removing semaphore\n");
  sem_desc = semget(KEY, 1, 0);
  int sem_val = semctl(sem_desc, 0, IPC_RMID);
  if (sem_val < 0) {
      printf("Initialize program first with: $ ./control -c\n");
      return;
  }
  printf("Semaphore removed: %d\n", sem_desc);
  mem_desc = shmget(KEY, 128 * sizeof(char), 0);
  // perror("shmget");
  printf("Shared memory segment removed: %d\n", shmctl(mem_desc, IPC_RMID, 0));
  // perror("shmctl");
  // remove file after showing it
  char buf[128];
  printf("The Story:\n");
  // FILE * doesn't seem to work as an initializer and stderr isn't used
  stderr = fopen(FILE, "r");
  // loop through and print out each line
  while(fgets(buf, sizeof(buf), stderr))
    printf("\t%s", buf);
  printf("Removed story: %d\n", remove(FILE));
}


int main(int c, const char *args[]) {
  // loop through args and check for flags and run corresponding helper
  int i;
  for(i = 0; i < c; i ++){
    if (strcmp(args[i], "-c") == 0){
      create(args[++i]);
      return 0;
    }else if (strcmp(args[i], "-v") == 0){
      view();
      return 0;
    }else if (strcmp(args[i], "-r") == 0){
      rem();
      return 0;
    }
  }
  printf("-c for creating -v for viewing -r for removing\n");
}
