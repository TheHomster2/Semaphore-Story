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
  // semaphore creation
  int sid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);

  // check for error
  if(sid == -1){
    printf("semaphore already created\n");
    return;
  }

  // semaphore val
  semctl(sid, 0, SETVAL, 1);
  // perror("semctl");
  printf("semaphore created: %d\n", sid);

  // shared memory creation
  int shmd = shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  printf("shared memory created: %d\n", shmd);
  printf("value set: %d\n", semctl(sid, 0, SETVAL, 1));

  // file creation
  int fd = open(FILE, O_CREAT | O_TRUNC | O_EXCL, 0644);
  close(fd);
  printf("file created:%d\n", fd);
}

// helper for view flag
void view() {
  // check for error
  if (semget(KEY, 1, 0)){
    printf("initialize first\n");
    return;
  }

  // print out story
  char buf[128];
  printf("The Story:\n");
  // FILE * doesn't seem to work as an initializer and stderr isn't used
  stderr = fopen(FILE, "r");
  while(fgets(buf, sizeof(buf), stderr))
    printf("\t%s", buf);
}

// helper for remove flag
void rem() {
  // removing semaphore
  int sid = semget(KEY, 1, 0);
  //check for error
  if (semctl(sid, 0, IPC_RMID){
      printf("Initialize program first with: $ ./control -c\n");
      return;
  }
  printf("Semaphore removed: %d\n", sid);

  // removing shared memory
  shmd = shmget(KEY, sizeof(int), 0);
  // perror("shmget");
  printf("Shared memory segment removed: %d\n", shmctl(shmd, IPC_RMID, 0));
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
  // loop through args and check for first flag
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
