#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"

symtab * init_st() {
   symtab * st = malloc(sizeof(symtab));
   st->height = 0;
   return st;
}

int height(symtab * st) {
   return st->height;
}

int push(symtab * st, variable var) {
   symcell * temp = malloc(sizeof(symcell));
   temp->var = var;
   temp->next = st->begin;
   st->begin = temp;
   return st->height++;
}

int pop(symtab * st, symcell * c) {
   if (height(st) == 0) {
      return -1;
   }
   c->next = st->begin->next;
   c->var = st->begin->var;
   symcell * tmp = st->begin->next;
   free(st->begin);
   st->begin = tmp;
   st->height--;
   return 0;
}

int get_sym_address(symtab * st, char* name) {
   symcell * temp = st->begin;
   while (temp != NULL && strcmp(temp->var.name,name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      return temp->var.add;
   }
}

int add_sym(symtab * st, enum type t, char* name, unsigned short depth) {
   if (get_sym_address(st, name) == -1) {
      variable var = {strdup(name), height(st), t, depth};
      return push(st, var);
   }
   return -1;
}

unsigned short get_tmp(symtab * st, unsigned short offset) {
   return height(st)+offset;
}

unsigned short is_tmp(symtab * st, unsigned short add) {
   return height(st) <= add;
}

int remove_depth(symtab * st, unsigned short depth) {
   symcell * temp = malloc(sizeof(symcell));
   unsigned short ret = pop(st, temp);
   while (ret == 0 && temp->var.depth >= depth) {
      ret = pop(st, temp);
   }
   if (ret == 0) {
      push(st, temp->var);
   }
   free(temp);
   return 0;
}
