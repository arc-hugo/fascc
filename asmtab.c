#include <stdlib.h>
#include "asmtab.h"

asmtab * init_at() {
   asmtab * at = malloc(sizeof(asmtab));
   at->size = 0;
   return at;
}

int size(asmtab * at) {
   return at->size;
}

int add(asmtab * at, inst ins) {
   if (size(at) == 0) {
      at->begin = malloc(sizeof(asmcell));
      at->end = at->begin;
      at->begin->ins = ins;
   } else {
      at->end->next = malloc(sizeof(asmcell));
      at->end = at->end->next;
      at->end->ins = ins;
   }
   at->size++;
   return 0;
}

int add_asm(asmtab * at, enum op op, unsigned short op0, unsigned short op1, unsigned short op2) {
   inst ins = { op, op0, op1, op2 };
   return add(at, ins);
}
