#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>

#define MAX_TOKENS 10

void initialize();
void terminal();

void getCommandline();
void tokenize();
void commandline_handler();

void menu();
void clear();
void time();
void showregisters();
void test_exception();
void fontsize();
void snake();
void beep();
void zoomin();
void zoomout();
void exitShell();
void invalid_command();

void test_opcode_exep();
void test_divzero_exep();

void printHeader();
void printDashLine();
void printMenu();
void ps();
void loop();
void nice();
void kill();
void block();
void runPhylo();

#endif