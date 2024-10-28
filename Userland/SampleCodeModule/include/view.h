#ifndef VIEW_H
#define VIEW_H

#define MAX_BUFFER 256
#define MAX_TOKENS 10

void initialize();
void terminal();

void getCommandline();
void commandline_handler();

void help();
void clear();
void changeusername();
void whoami();
void time();
void showregisters();
void test_exception();
void snake();
void exit();
void invalid_command();

void test_opcode_exep();
void test_divzero_exep();

void printMenu();

#endif