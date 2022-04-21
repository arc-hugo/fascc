#ifndef SYMTAB_H__
#define SYMTAB_H__

#include "function.h"

struct funcell;
typedef struct funcell {
   function fun;
   struct funcell* next;
} funcell;

typedef struct {
   unsigned int height;
   funcell* begin;
} funtab;

funtab * init_ft();

int add_fun(funtab * st, function *fun);
int get_fun(funtab * st, char* name, function* fun);

#endif
