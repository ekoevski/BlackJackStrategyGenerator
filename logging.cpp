#include "logging.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <filesystem>

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
void purple() {
  printf("\033[0;35m");
}

void cyan() {
  printf("\033[0;36m");
}

void delete_log_files()
{
  remove("OUTPUT_LOG/output0.txt");  
  remove("OUTPUT_LOG/output1.txt");
  remove("OUTPUT_LOG/output.txt");
}

// Logging functions
void log_1(const char * message, const char *file, int line,...)
{
    char buf[200];
    char buf2[100];
    va_list args;

    snprintf(buf2, 50, "%s:%d: ", file, line);

    // Print first part colored (file and line)
    green();
    printf("%25s", buf2);
    cyan();

    // Print whatever message user wants after
    strcpy(buf, message);       // Copy user message
    va_start(args, line);    // Collect variatric arguments (notice you just need last argument listed)
    vprintf(buf, args);         // Print variatric arguments with buffer message
    va_end(args);

    reset();
    //Endline
    printf("\n");

} 

void log_0(const char * message, const char *file, int line,...)
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

void log_flat(const char * message, const char *file, int line,...)
{
    char buf[200];
    char buf2[100];
    va_list args;

    snprintf(buf2, 50, "%s:%d: ", file, line);

    // Print first part colored (file and line)
    green();
    printf("%25s", buf2);
    cyan();

    // Print whatever message user wants after
    strcpy(buf, message);       // Copy user message
    va_start(args, line);    // Collect variatric arguments (notice you just need last argument listed)
    vprintf(buf, args);         // Print variatric arguments with buffer message
    va_end(args);

    reset();
    //Endline
} 


// Logging function
void log_error(const char * message, const char *file, int line,...)
{
    char buf[200];
    char buf2[100];
    va_list args;

    snprintf(buf2, 50, "%s:%d: ", file, line);

    // Print first part colored (file and line)
    cyan();
    printf("%25s", buf2);
    red();

    // Print whatever message user wants after
    strcpy(buf, message);       // Copy user message
    va_start(args, line);    // Collect variatric arguments (notice you just need last argument listed)
    vprintf(buf, args);         // Print variatric arguments with buffer message
    va_end(args);
    reset();
    //Endline
    printf("\n");

} 

void writeFile(const char *s_out)
{
  FILE*fp;
  fp = fopen("OUTPUT_LOG/output0.txt", "a+");
  fwrite(s_out, sizeof(char), strlen(s_out), fp);
  fclose(fp);
}

// Logging function
void vlog_0(const char * message,...)
{
    char s_out[1000];
    va_list args;

    red();
    // Print whatever message user wants after
    va_start(args, message);    // Collect variatric arguments (notice you just need last argument listed)
    vsprintf(s_out, message, args);         // Print variatric arguments with buffer message
    va_end(args);
    writeFile(s_out);

    reset();
} 

void writeFile_1(const char *s_out)
{
  FILE*fp;
  fp = fopen("OUTPUT_LOG/output1.txt", "a+");
  fwrite(s_out, sizeof(char), strlen(s_out), fp);
  fclose(fp);
}

// Logging function
void vlog_1(const char * message,...)
{
    char s_out[1000];
    va_list args;

    red();
    // Print whatever message user wants after
    va_start(args, message);    // Collect variatric arguments (notice you just need last argument listed)
    vsprintf(s_out, message, args);         // Print variatric arguments with buffer message
    va_end(args);
    writeFile_1(s_out);

    reset();
} 