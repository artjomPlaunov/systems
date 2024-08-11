// my_tee command
// read standard input until end-of-file, writing a copy of the input 
// to standard output and to the file named in the command line argument.

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif


int main(int argc, char *argv[]) {
  int outputFd, openFlags;
  mode_t filePerms;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s tee file\n", argv[0]);

  openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open(argv[1], openFlags, filePerms);
  if (outputFd == -1)
    errExit("opening file %s", argv[1]);

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (write(outputFd, buf, numRead) != numRead)
      fatal("write() returned error or partial write occured");
    if (write(STDOUT_FILENO, buf, numRead) != numRead)
      fatal("write() returned error or partial write occured");
  }
  if (numRead == -1)
    errExit("read");
  if (close(outputFd) == -1)
    errExit("close output");
  exit(EXIT_SUCCESS);
}
