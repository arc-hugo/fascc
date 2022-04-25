#ifndef FUNTAB_H__
#define FUNTAB_H__

#include "function.h"

struct funcell;
typedef struct funcell {
   function fun;
   struct funcell* next;
} funcell;

typedef struct funtab {
   unsigned int height;
   funcell* begin;
} funtab;

funtab * init_ft();

int add_fun(funtab * ft, function *fun);
int get_fun(funtab * ft, char* name, function* fun);

void set_main_fun(funtab* ft);

#endif
