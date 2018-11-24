#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include "parseargs.h"
#include "io.h"

// Function where the system command is executed
void run(char** args, int argc) {
  // Forking a child
  int pid = fork();
  if(pid == -1) {
    printf("fysh: failed to fork child\n");
  }
  else if(pid == 0) {
    if(!argc) { return; }
    if(execvp(args[0], args) < 0) {
      printf("fysh: failed to execute %s\n", args[0]);
    }
    exit(0);
  } else {
    // waiting for child to terminate
    wait(NULL);
    return;
  }
}

void printprompt() {
  char curD[100];
  getcwd(curD, sizeof(curD));
  printf("\x1B[34m");
  printf("%s", curD);
  printf("$ ");
  printf("\x1B[0m");
}

int main() {
  //execvp(args[0], args);
  while(1) {
    unsigned char line[1000];
    printprompt();
    liveRead(line, 1000);
    if(!strcmp(line, "")) {
    
    } else {

      char ** semiColons = parse_argsSemiColon(line);
      int i = 0;
      while(semiColons[i]) {
        int argc;
        char ** argv = parse_args(&argc, semiColons[i++]);
        if(strcmp(argv[0], "exit") == 0) {
          exit(0);
        }
        else if(strcmp(argv[0], "cd") == 0) {
          if(argc - 1 > 2) {
            printf("fysh: cd takes 1 argument, %d found\n", argc - 1);
          }
          int stat = chdir(argv[1]);
          if(stat == -1) {
            printf("fysh: cd: %s\n", strerror(errno));
          }
        }
        else {
          run(argv, argc);
        }
      }
    }
  }

  //char * str = args[0];
  //printdir(str);
  return 0;
}
