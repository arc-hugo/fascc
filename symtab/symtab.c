#include <stddef.h>
#include <stdlib.h>
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

int pop(symtab * st, cell ** c) {
   if (height(st) == 0) {
      return -1;
   }
   *c = st->begin;
   st->begin = st->begin->next;
   return 0;
}
