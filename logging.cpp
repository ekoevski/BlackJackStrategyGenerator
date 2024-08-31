#include "logging.h"
#include <string.h>
#include <stdio.h>




// Color change for printf
void red () {
  printf("\033[1;31m");
}

void green() {
  printf(" \033[0;32m");
}

void reset () {
  printf("\033[0m");
}

void blue () {
  printf("\033[0;34m");
}

void yellow() {
  printf("\033[1;33m");
}



// Logging function
void log_function(const char * message, const char *file, int line,...)
{
    char buf[200];
    char buf2[100];
    va_list args;

    snprintf(buf2, 50, "%s:%d: ", file, line);

    // Print first part colored (file and line)
    yellow();
    printf("%25s", buf2);
    reset();

    // Print whatever message user wants after
    strcpy(buf, message);       // Copy user message
    va_start(args, line);    // Collect variatric arguments (notice you just need last argument listed)
    vprintf(buf, args);         // Print variatric arguments with buffer message
    va_end(args);

    //Endline
    printf("\n");

} 