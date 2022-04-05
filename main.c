#include "argparse.h"



int main(int argc, char** argv)
{
    printf("hello world!\n");

    argparse_register_int("apple", 'a', true, 0, "this is the apple argument");
    argparse_print_help(NULL);
    return 0;
}