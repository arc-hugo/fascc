#include <stdlib.h>
#include <string.h>
#include "symtab.h"

symtab * init_stack() {
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
      st->height++;
   }
   return 0;
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
   variable var = {strdup(name), height(st), t, depth};
   cell * c = malloc(sizeof(cell));
   c->var = var;
   return push(st, c);
}

int remove_depth(symtab * st, unsigned short depth) {
   cell * temp = malloc(sizeof(cell));
   unsigned short ret = pop(st, temp);
   while (ret == 0 && temp->var.depth >= depth) {
      pop(st, temp);
   }
   if (ret == 0) {
      push(st, temp);
   }
   return 0;
}
