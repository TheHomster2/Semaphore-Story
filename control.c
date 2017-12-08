#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define KEY 1729

/*
  Charles Weng
  Systems pd 4
  HW 14 - Get Sem Control!
  2017 - 12 - 6

  side note - from my testings, semget seems to be returning 0 instead of the sid
    not sure if it's a windows thing or i did something wrong
*/

// helper for create flag
void create(const char * x) {
  // get the value user wants for val
  int semVal;
  sscanf(x, "%d", &semVal);
  int sid = semget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
  // check for error
  if(sid == -1){
    printf("semaphore already created\n");
    return;
  }

  semctl(sid, 0, SETVAL, semVal);
  printf("semaphore created: %d\n", sid);
  printf("value set: %d\n", semctl(sid, 0, GETVAL));
}

// helper for view flag
void view() {
  int sid = semget(KEY, sizeof(int), 0600);
  printf("semaphore value: %d\n", semctl(sid, 0, GETVAL));
}

// helper for remove flag
void rem() {
  int sid = semget(KEY, 4, 0600);
  printf("Semaphore removed: %d\n", semctl(sid, 0, IPC_RMID));
}


int main(int c, const char *args[]) {
  // loop through args and check for flags and run corresponding helper
  int i;
  for(i = 0; i < c; i ++){
    if (strcmp(args[i], "-c") == 0)
      return create(args[++i]);
    else if (strcmp(args[i], "-v") == 0)
      return view();
    else if (strcmp(args[i], "-r") == 0)
      return rem();
    }
    printf("\n", );
}
