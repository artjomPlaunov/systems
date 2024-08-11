// my_tee command
// read standard input until end-of-file, writing a copy of the input 
// to standard output and to the file named in the command line argument.

// Default is to overwrite any existing file passed as an argument.
// tee -a file will append to the file if it already exists (TODO).

#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

static void
usageError(char *progName, char *msg, int opt) {
  if (msg != NULL && opt != 0)
    fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
  fprintf(stderr, "Usage: %s [-a] file\n", progName);
  exit(EXIT_FAILURE);
}



int main(int argc, char *argv[]) {
  // Process options.
  int opt, afnd;
  afnd = 0;
  
  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a': afnd = 1; break;
    case '?': usageError(argv[0], "Unrecognized option", optopt); break;
    default:  fatal("Unexpected case in switch()");
    }
  }

  if (optind == argc)
    usageError(argv[0], "", 0);
		       
  int outputFd, openFlags;
  mode_t filePerms;
  ssize_t numRead;
  char buf[BUF_SIZE];

  // If -a flag is found, set flag to append,
  // otherwise set to truncate.
  openFlags = O_CREAT | O_WRONLY;
  if (afnd)
    openFlags |= O_APPEND;
  else
    openFlags |= O_CREAT;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open(argv[optind], openFlags, filePerms);
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
