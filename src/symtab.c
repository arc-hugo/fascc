#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"

symtab * init_st() {
   symtab * st = malloc(sizeof(symtab));
   st->height = 0;
   return st;
}

int height_st(symtab * st) {
   return st->height;
}

int push_st(symtab * st, variable var) {
   symcell * temp = malloc(sizeof(symcell));
   temp->var = var;
   temp->next = st->begin;
   st->begin = temp;
   return st->height++;
}

int pop_st(symtab * st, symcell * c) {
   if (height_st(st) == 0) {
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

int get_sym_address(symtab * st, char* name, unsigned short * cst) {
   symcell * temp = st->begin;
   while (temp != NULL && strcmp(temp->var.name,name) != 0) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      *cst = temp->var.cst;
      return temp->var.add;
   }
}

int is_present(symtab * st, char* name) {
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
   if (is_present(st, name) == -1) {
      variable var = {strdup(name), height_st(st), t, depth, 0};
      return push_st(st, var);
   }
   return -1;
}

int add_cst(symtab * st, enum type t, char* name, unsigned short depth) {
   if (is_present(st, name) == -1) {
      variable var = {strdup(name), height_st(st), t, depth, 1};
      return push_st(st, var);
   }
   return -1;
}

unsigned short get_tmp(symtab * st, unsigned short offset) {
   return height_st(st)+offset;
}

unsigned short is_tmp(symtab * st, unsigned short add) {
   return height_st(st) <= add;
}

int remove_depth(symtab * st, unsigned short depth) {
   symcell * temp = malloc(sizeof(symcell));
   unsigned short ret = pop_st(st, temp);
   while (ret == 0 && temp->var.depth >= depth) {
      ret = pop_st(st, temp);
   }
   if (ret == 0) {
      push_st(st, temp->var);
   }
   free(temp);
   return 0;
}
