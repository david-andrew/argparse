#ifndef ARGPARSE_H
#define ARGPARSE_H


/**
 * Simple, light-weight, header-only library for parsing command-line args
 * 
 * Usage:
 * #include "argparse.h"
 * <register arguments>
 * <collect argument values>
 */


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    __int_t,
    __float_t,
    __str_t,
    __flag_t,
} __argtype_t;

typedef struct {
    __argtype_t type;       // type of argument being stored
    char* long_name;        // name of argument, e.g. `--arg`. Possibly NULL
    char short_name;        // single letter name of argument, e.g. `-a`. Possibly 0
    bool required;
    union {
        char* s;
        int i;
        double f;
        bool b;
    } value;
    char* help;
} __arg_t;


#define __MAX_ARGS 32 //change if need more arguments
__arg_t __argparse_args[__MAX_ARGS];
int __argparse_count = 0;

void check_max_args()
{
    if (__argparse_count >= __MAX_ARGS)
    {
        printf("ERROR: too many arguments. Can only register up to %d arguments\n", __MAX_ARGS);
        exit(1);
    }
}

int argparse_register_int(char* long_name, char short_name, bool required, int dfault, char* help)
{
    check_max_args();
    //TODO->safety checks:
    //  - at least either a short_name or long_name
    //  - names have never been used before
    __argparse_args[__argparse_count] = (__arg_t){__int_t, long_name, short_name, required, {.i=dfault}, help};
    return __argparse_count++;
}

int argparse_register_str()
{}

int argparse_register_flag()
{}

void argparse_parse_args(int argc, char** argv)
{
    for (int i = 0; i < __argparse_count; i++) 
    {
        __arg_t arg = __argparse_args[i];
        //TODO
        //for (int j = 0; j < argc; j++)
        //{
        //    if matches short/long name, 
        //        update value based on type
        //        switch (arg.type)
        //            case __int_t: arg.value.i = atoi(argv[j]); break;
        //            case __str_t: arg.value.s = argv[j]; break;
        //            case __float_t: arg.value.f = strtod(argv[j]); break;
        //}
    }
}

int __argparse_get_idx_by_name(char* long_name, char short_name)
{
    if (long_name == NULL && short_name == 0)
    {
        printf("ERROR: at least long_name, or short_name must be provided\n");
        exit(1);
    }
    for (int i = 0; i < __argparse_count; i++) 
    {
        __arg_t arg = __argparse_args[i];
        if ((arg.short_name != 0 && short_name == arg.short_name) ||
            (long_name != NULL && arg.long_name != NULL && strcmp(long_name, arg.long_name) == 0))
        {
            return i;
        }
    }
    if (long_name != NULL)
        printf("ERROR: unable to find argument --%s\n", long_name);
    else
        printf("ERROR: unable to find argument -%c\n", short_name);
    exit(1);
}
int argparse_get_int_by_id(int i) { return __argparse_args[i].value.i; }
char* argparse_get_str_by_id(int i) { return __argparse_args[i].value.s; }
int argparse_get_int_by_name(char* long_name, char short_name)
{
    int i = __argparse_get_idx_by_name(long_name, short_name);
    return argparse_get_int_by_id(i);
}
char* argparse_get_str_by_name(char* long_name, char short_name)
{
    int i = __argparse_get_idx_by_name(long_name, short_name);
    return argparse_get_str_by_id(i);
}

void argparse_print_help(char* prog_name)
{
    printf("usage: ./");
    fputs(prog_name != NULL ? prog_name : "a.out", stdout);
    for (int i = 0; i < __argparse_count; i++)
    {
        putchar(' ');
        __arg_t arg = __argparse_args[i];
        fputs(arg.required ? "" : "[", stdout);
        // if (!arg.flag)
        {
            if (arg.long_name != NULL)
                printf("--%s", arg.long_name);
            if (arg.long_name != NULL && arg.short_name != 0)
                fputs(", ", stdout);
            if (arg.short_name != 0)
                printf("-%c", arg.short_name);
        }
        fputs(arg.required ? "" : "]", stdout);
    }
    fputs("\n\n", stdout);

    for (int i = 0; i < __argparse_count; i++)
    {
        fputs("  ", stdout);
        __arg_t arg = __argparse_args[i];
        if (arg.short_name != 0)
            printf("-%c", arg.short_name);
        if (arg.long_name != NULL && arg.short_name != 0)
            fputs(", ", stdout);
        if (arg.long_name != NULL)
            printf("--%s", arg.long_name);
        
        //TODO->count characters printed so far, and pad to specific width
        fputs("    ", stdout); //dumb padding
        fputs(arg.help, stdout);
        putchar('\n');
    }
}


#endif