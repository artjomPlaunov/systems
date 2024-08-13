// Implementations of dup and dup2 functions.


#include <unistd.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int my_dup(int oldfd) {
  int newfd;
  if ((newfd = fcntl(oldfd, F_DUPFD)) == -1) {
    errExit("my_dup(): fcntl(%d, F_DUPFD) error", oldfd);
  }
  return newfd;
}

int my_dup2(int oldfd, int newfd) {
  if (fcntl(oldfd, F_GETFL) == -1)
    errExit("my_dup2(): fcntl(%d, F_GETFL) error", oldfd);
  if (oldfd == newfd)
    return oldfd;
  if (fcntl(newfd, F_GETFL) >= 0) {
    int ignore_err = close(newfd);
  }
  if (fcntl(oldfd, F_DUPFD, newfd) != newfd)
    errExit("my_dup2(): fcntl(%d, F_DUPFD, %d error", oldfd, newfd);
  return newfd;
  

}


// Driver to test my_dup and my_dup2
int main(int arc, char *argv[]) {
  char buf[BUF_SIZE];
  // Check error case for dup.
  //int newfd = my_dup(923);

  // test out my_dup().
  int newfd = open("dummy.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  if (newfd == -1)
    errExit("opening file dummy.txt");
  int samefd = my_dup(newfd);
  ssize_t numRead;

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (write(newfd, buf, numRead) != numRead)
      fatal("write() returned error or partial write occured");
    if (write(samefd, buf, numRead) != numRead)
      fatal("write() returned error or partial write occured");
  }

  int dupfd = my_dup2(newfd, 993);
  
  
  if (numRead == -1)
    errExit("read");
  if (close(newfd) == -1)
    errExit("close output");
  if (close(samefd) == -1)
    errExit("close output");
  if (close(dupfd) == -1)
    errExit("close output");
  exit(EXIT_SUCCESS);
  
}
