#include <stdlib.h>
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

int push(symtab * st, cell * c) {
   if (c != NULL) {
      cell * temp = st->begin;
      c->next = temp;
      st->begin = c;
      return st->height++;
   }
   return -1;
}

int pop(symtab * st, cell * c) {
   if (height(st) == 0) {
      return -1;
   }
   c = st->begin;
   st->begin = st->begin->next;
   st->height--;
   return 0;
}

int get_address(symtab * st, char* name) {
   cell * temp = st->begin;
   while (temp != NULL && !strcmp(temp->var.name,name)) {
      temp = temp->next;
   }
   if (temp == NULL) {
      return -1;
   } else {
      return temp->var.add;
   }
}

int add_sym(symtab * st, enum type t, char* name, unsigned short depth) {
   if (get_address(st, name) == -1) {
      variable var = {strdup(name), height(st), t, depth};
      cell * c = malloc(sizeof(cell));
      c->var = var;
      return push(st, c);
   }
   return -1;
}

unsigned short get_tmp(symtab * st, unsigned short offset) {
   return height(st)+offset;
}

int is_tmp(symtab * st, unsigned short add) {
   return height(st) <= add;
}

int remove_depth(symtab * st, unsigned short depth) {
   cell * temp = malloc(sizeof(cell));
   unsigned short ret = pop(st, temp);
   while (ret == 0 && temp->var.depth >= depth) {
      free(temp);
      ret = pop(st, temp);
   }
   if (ret == 0) {
      push(st, temp);
   }
   return 0;
}
