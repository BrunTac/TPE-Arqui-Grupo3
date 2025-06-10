#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>

#define MAX_TOKENS 10
#define MAX_PIPES 10
#define DEFAULT_PRIORITY 1

void initialize();
void terminal();

void getCommandline();
void tokenize();
void commandline_handler();
void pipe_handler();
int8_t getCommand(char * name);

// BUILT-IN COMMANDS
void clear();
void time();
void showregisters();
void zoomin();
void zoomout();
void nice(uint64_t argc, char * argv[]);
void kill(uint64_t argc, char * argv[]);
void block(uint64_t argc, char * argv[]);
void exitShell();

// PROCESS COMMANDS
void menu();
void runSnake();
void runPhylo(uint64_t argc, char * argv[]);
void runLoop();
void runPs();
void runCat();
void runWc();
void runFilter();
void runViewMem();
void runTestprocesses();
void runTestprio();
void runTestmm();

void printHeader();
void invalid_command();

int checkArguments(int arguments);
void runTestsync();
void runTestnosync();

#endif