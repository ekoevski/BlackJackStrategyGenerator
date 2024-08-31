#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <iostream>
#include <unistd.h> 
#include <cstdarg>
// Log control

using namespace std;

//To endable logging
#define LOG_0(message,file, line, ...) log_0(message,file, line, __VA_ARGS__);
//#define LOG_0(message,file, line, ...) 0;    // To disable logging 
#define LOG_1(message,file, line, ...) log_1(message,file, line, __VA_ARGS__);
//#define LOG_ERROR(message,file, line, ...) 0;    // To disable logging 
#define LOG_ERROR(message,file, line, ...) log_error(message,file, line, __VA_ARGS__);
//#define LOG_ERROR(message,file, line, ...) 0;    // To disable logging 



void log_0(const char * message, const char * file, int line,...);
void log_1(const char * message, const char * file, int line,...);
void log_error(const char * message, const char * file, int line,...);
#endif //LOGGING_H
