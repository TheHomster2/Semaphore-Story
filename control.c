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

/*
  Anthony Hom Charles Weng
  Systems pd 4
  HW 15 - Semaphone
  2017 - 12 - 11

  below code is from charles' repo
*/

// helper for create flag; creates the semaphore if not already created
void create() {
  int sid = semget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  int shmd = shmget(KEY, 128 * sizeof(char), IPC_CREAT | IPC_EXCL);
  perror("shmget");

  // check for error
  if(sid == -1){
    printf("semaphore already created\n");
    return;
  }
  if(shmd == -1){
    printf("shared memory already created\n");
    return;
  }
  printf("semaphore created: %d\n", sid);
  printf("shared memory created: %d\n", shmd);
  printf("value set: %d\n", semctl(sid, 0, SETVAL, 1));

  int fd = open(FILE, O_CREAT | O_TRUNC, 0644);
  close(fd);
  printf("file created\n");
}

// helper for view flag; returns
void view() {
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
  // remove semaphore
  int sid = semget(KEY, 4, 0600);
  printf("semaphore removed: %d\n", semctl(sid, 0, IPC_RMID));
  // remove shared memory
  int shmd = shmget(KEY, 4, 0600);

  printf("shared memory removed: %d\n", shmctl(shmd, IPC_RMID, 0));
  perror("shmctl");
  // remove file after showing it
  view();
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
