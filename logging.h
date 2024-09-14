#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <iostream>
#include <unistd.h> 
#include <cstdarg>

using namespace std;


// Use these as build switches
#define DEBUG                                   0   // set to 0 to turn off debug logs (much faster)
#define HARD_STRATEGY                           1
#define SOFT_STRATEGY                           1
#define SPLIT_STRATEGY                          1

// SELECT ONLY ONE OF THESE
#define BASIC_STRATEGY_ZERO                     0
#define BASIC_STRATEGY_STANDARD                 1

#define STAY                                    0
#define HIT                                     1
#define DOUBLE                                  2
#define SPLIT                                   4

#define HARD_HAND_MODE                          0
#define SOFT_HAND_MODE                          1
#define SPLIT_HAND_MODE                         3




// Always log
#define VLOG_0(message,...)                     vlog_0(message,__VA_ARGS__) 
#define VLOG_2(message,...)                     vlog_2(message,__VA_ARGS__) 

#if DEBUG
#define VLOG_1(message,...)                     vlog_1(message,__VA_ARGS__)       // Prints basic strategy vector in OUTPUT folder
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

void delete_log_files();

void log_0(const char * message, const char * file, int line,...);
void log_1(const char * message, const char * file, int line,...);
void log_flat(const char * message, const char * file, int line,...);
void log_error(const char * message, const char * file, int line,...);

void vlog_0(const char * message,...);  // Output to file
void vlog_1(const char * message,...);  // Output to file
void vlog_2(const char * message,...);  // Output to file

// Separate running log to file




#endif //LOGGING_H
