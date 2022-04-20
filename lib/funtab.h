#ifndef SYMTAB_H__
#define SYMTAB_H__

#include "type.h"

typedef struct {
   char* name;
   unsigned int add;
   enum type t;
   unsigned int argc;
} fonction;

struct funcell;
typedef struct funcell {
   fonction fun;
   struct funcell* next;
} funcell;

typedef struct {
   unsigned int height;
   funcell* begin;
} funtab;

funtab * init_ft();

int add_fun(funtab * st, enum type t, char* name, unsigned int add, unsigned int argc);
int get_fun_address(funtab * st, char* name);
int get_argc(funtab * st, char* name);

#endif
