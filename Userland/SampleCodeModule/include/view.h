#ifndef VIEW_H
#define VIEW_H


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
void snake();
void beep();
void exit();
void invalid_command();

void test_opcode_exep();
void test_divzero_exep();

void printHeader();
void printDashLine();
void printMenu();

#endif