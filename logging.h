#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <iostream>
#include <unistd.h> 
#include <cstdarg>

using namespace std;

#define DEBUG                                   1   // comment out to turn off debug mode

#define STAY                                    0
#define HIT                                     1
#define DOUBLE                                  2
#define SPLIT                                   4

#define HARD_HAND_MODE                          0
#define SOFT_HAND_MODE                          1
#define SPLIT_HAND_MODE                         3

#if DEBUG
#define LOG_0(message,file, line, ...)          log_0(message,file, line, __VA_ARGS__)
#define LOG_1(message,file, line, ...)          log_1(message,file, line, __VA_ARGS__)
#define LOG_FLAT(message,file, line, ...)       log_flat(message,file, line, __VA_ARGS__)
#define LOG_ERROR(message,file, line, ...)      log_error(message,file, line, __VA_ARGS__)   
#else
#define LOG_0(message,file, line, ...)          0
#define LOG_1(message,file, line, ...)          0
#define LOG_ERROR(message,file, line, ...)      0
#define LOG_FLAT(message,file, line, ...)       0
#endif

void log_0(const char * message, const char * file, int line,...);
void log_1(const char * message, const char * file, int line,...);
void log_flat(const char * message, const char * file, int line,...);
void log_error(const char * message, const char * file, int line,...);
#endif //LOGGING_H
