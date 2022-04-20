#include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funtab.h"

funtab * init_ft() {
   funtab * ft = malloc(sizeof(funtab));
   ft->height = 0;
   return ft;
}

int height(funtab * ft) {
   return ft->height;
}

int push(funtab * ft, fonction fun) {
   funcell * temp = malloc(sizeof(funcell));
   temp->fun = fun;
   temp->next = ft->begin;
   ft->begin = temp;
   return ft->height++;
}

int pop(funtab * ft, funcell * c) {
   if (height(ft) == 0) {
      return -1;
   }
   c->next = ft->begin->next;
   c->fun = ft->begin->fun;
   funcell * tmp = ft->begin->next;
   free(ft->begin);
   ft->begin = tmp;
   ft->height--;
   return 0;
}

int get_fun_address(funtab *ft, char *name) {
   funcell * temp = ft->begin;
   while (temp != NULL && strcmp(temp->fun.name, name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      return temp->fun.add;
   }
}

int get_argc(funtab *ft, char *name) {
   funcell * temp = ft->begin;
   while (temp != NULL && strcmp(temp->fun.name, name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      return temp->fun.argc;
   }
}

int add_fun(funtab *ft, enum type t, char *name, unsigned int add, unsigned int argc) {
   if (get_fun_address(ft,name) == -1) {
         fonction fun = {strdup(name),add,t,argc};
         return push(ft, fun);
   }
   return -1;
}
