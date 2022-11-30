#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

struct shmseg {
    int cnt;
    int complete;
    char buf[BUF_SIZE];
};

int fill_buffer(char *bufptr, int size);

int main(int, char**) {
    int shmid, numtimes;
    struct shmseg *shmp;
    char *bufptr;
    int spaceAvailable;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }
    shmp = (struct shmseg *) shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    bufptr = shmp -> buf;
    spaceAvailable = BUF_SIZE;
    for (numtimes = 0; numtimes < 5; numtimes++) {
        shmp -> cnt = fill_buffer(bufptr, spaceAvailable);
        shmp -> complete = 0;
        std::cout << "Writing Process: Shared Memory Write: Wrote " << shmp->cnt << "bytes" << std::endl;
        bufptr = shmp -> buf;
        spaceAvailable = BUF_SIZE;
        sleep(3);
    }
    std::cout << "Writing Process: Wrote " << numtimes << " times\n";
    shmp -> complete = 1;

    if (shmdt(shmp) == -1) {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    std::cout << "Writing Process: Complete\n";
    return 0;
}

int fill_buffer(char * bufptr, int size) {
   static char ch = 'A';
   int filled_count;
   
   //printf("size is %d\n", size);
   memset(bufptr, ch, size - 1);
   bufptr[size-1] = '\0';
   if (ch > 122)
   ch = 65;
   if ( (ch >= 65) && (ch <= 122) ) {
      if ( (ch >= 91) && (ch <= 96) ) {
         ch = 65;
      }
   }
   filled_count = strlen(bufptr);
   
   //printf("buffer count is: %d\n", filled_count);
   //printf("buffer filled is:%s\n", bufptr);
   ch++;
   return filled_count;
}
