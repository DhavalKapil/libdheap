/**
 * Test file
 * Executes a vulnerable double free program using execve
 */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define VULN_PROG "./double_free"

int main() {
  char *argv[2];
  argv[0] = VULN_PROG;
  argv[1] = 0;
  int err = execve(VULN_PROG, argv, 0);
  if (err == -1) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
  }
  return 0;
}
