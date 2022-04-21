#include <stdlib.h>
#include <string.h>

#include "function.h"

function * init_fun(char* name, unsigned int add, enum type t) {
   function* fun = malloc(sizeof(function));
   fun->name = strdup(name);
   fun->add = add;
   fun->t = t;
   fun->argc = 0;
   fun->begin = NULL;
   return fun;
}

int add_arg(function* fun, char *name, enum type t) {
   if (fun->begin != NULL) {
      argcell* temp = fun->begin;
      while (temp->next != NULL) {
         if (strcmp(fun->name, name) == 0) {
            return -1;
         }
         temp = temp->next;
      }
      temp->next = malloc(sizeof(argcell));
      argument arg = {strdup(name),t};
      temp->next->arg = arg;
      temp->next->next = NULL;
   } else {
      fun->begin = malloc(sizeof(argcell));
      argument arg = {strdup(name),t};
      fun->begin->arg = arg;
      fun->begin->next = NULL;
   }
   fun->argc++;
   return fun->argc;
}
