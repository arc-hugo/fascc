#ifndef FUNCTION_H__
#define FUNCTION_H__

#include "type.h"

typedef struct {
   char* name;
   enum type t;
} argument;

struct argcell;
typedef struct argcell {
   argument arg;
   struct argcell* next;
} argcell;

typedef struct {
   char* name;
   unsigned int add;
   enum type t;
   unsigned int argc;
   struct argcell* begin;
} function;

function * init_fun(char* name, unsigned int add, enum type t);
int add_arg(function* fun, char *name, enum type t);

#endif
