#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <iostream>
#include <unistd.h> 
#include <cstdarg>
// Log control

using namespace std;

//To endable logging
#define LOG_0(message,file, line, ...) log_function(message,file, line, __VA_ARGS__);
//#define LOG_0(message,file, line, ...) 0;    // To disable logging 


void log_function(const char * message, const char * file, int line,...);
#endif //LOGGING_H
