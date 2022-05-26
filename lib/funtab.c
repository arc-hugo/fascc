#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "function.h"
#include "funtab.h"

funtab * init_ft() {
   funtab * ft = malloc(sizeof(funtab));
   ft->height = 0;
   return ft;
}

int height_ft(funtab * ft) {
   return ft->height;
}

int push_ft(funtab * ft, function *fun) {
   funcell * temp = malloc(sizeof(funcell));
   temp->fun = *fun;
   temp->next = ft->begin;
   ft->begin = temp;
   return ft->height++;
}

int get_fun(funtab *ft, char *name, function* fun) {
   funcell * temp = ft->begin;
   while (temp != NULL && strcmp(temp->fun.name, name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      fun->add = temp->fun.add;
      fun->name = temp->fun.name;
      fun->t = temp->fun.t;
      fun->argc = temp->fun.argc;
      fun->begin = temp->fun.begin;
      return temp->fun.add;
   }
}

int is_fun_present(funtab *ft, char* name) {
   funcell * temp = ft->begin;
   while (temp != NULL && strcmp(temp->fun.name, name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return 0;
   } else {
      return 1;
   }
}

int add_fun(funtab *ft, function *fun) {
   if (!is_fun_present(ft,fun->name)) {
      return push_ft(ft, fun);
   }
   return -1;
}

int set_main_fun(funtab *ft, unsigned int ln, enum type t) {
   if (!is_fun_present(ft, "main")) {
      funcell * temp = ft->begin;
      while (temp != NULL) {
         temp->fun.add++;
         temp = temp->next;
      }
      function *fun = init_fun("main", ln, t);
      return push_ft(ft, fun);
   }
   return -1;
}
