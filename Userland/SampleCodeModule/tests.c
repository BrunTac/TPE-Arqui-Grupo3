#include "tests.h"
#include "libc.h"

extern void opcodeError();
void test_opcode_exep();
void test_divzero_exep();

void test_exception(uint64_t argc, char * argv[]){
    char * arg = argv[1];
    if(strcmp(arg, "opcode") == 0){
        test_opcode_exep();
    }else if(strcmp(arg, "divzero") == 0){
        test_divzero_exep();
    }else{
        printf("%nInvalid Argument. Please enter one of the following options:%n");
        printf("1. opcode%n2. divzero%n");
    }
}
void test_opcode_exep(){
    opcodeError();
}
void test_divzero_exep(){
    int a = 5 / 0;
}