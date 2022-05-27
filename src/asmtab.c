#include <stdlib.h>
#include <stdio.h>
#include "asmtab.h"

asmtab * init_at() {
   asmtab * at = malloc(sizeof(asmtab));
   at->size = 0;
   return at;
}

unsigned int get_last_line(asmtab *at) {
   return at->size;
}

int add(asmtab * at, inst ins) {
   if (get_last_line(at) == 0) {
      at->begin = malloc(sizeof(asmcell));
      at->end = at->begin;
      at->begin->ins = ins;
      at->begin->line = 0;
      at->begin->previous = NULL;
      at->begin->next = NULL;
   } else {
      at->end->next = malloc(sizeof(asmcell));
      at->end->next->previous = at->end;
      at->end = at->end->next;
      at->end->next = NULL;
      at->end->ins = ins;
      at->end->line = get_last_line(at);
   }
   at->size++;
   return 0;
}

int remove_op(asmtab * at, enum op op, asmcell ** c) {
   asmcell * tmp = at->end;
   while (tmp != NULL && tmp->ins.op != op) {
      tmp = tmp->previous;
   }
   if (tmp != NULL) {
      tmp->previous->next = tmp->next;
      tmp->next->previous = tmp->previous;
      *c = tmp;
      at->size--;
      return tmp->line;
   }
   return -1;
}

int add_asm(asmtab * at, enum op op, unsigned int op0, unsigned int op1, unsigned int op2) {
   inst ins = { op, op0, op1, op2 };
   return add(at, ins);
}

int reduce_cop(asmtab * at) {
   if (at->end->ins.op == COP &&
         (at->end->previous->ins.op == AFC ||
          at->end->previous->ins.op == ADD ||
          at->end->previous->ins.op == SOU ||
          at->end->previous->ins.op == MUL ||
          at->end->previous->ins.op == DIV) &&
         at->end->previous->ins.op0 == at->end->ins.op1
      ) {
      at->end->previous->ins.op0 = at->end->ins.op0;
      asmcell * tmp = at->end;
      at->end = at->end->previous;
      free(tmp);
      at->end->next = NULL;
      at->end->line--;
      at->size--;
   }
   return 0;
}

int jump_if(asmtab *at, unsigned int pos, unsigned int ln) {
   asmcell* tmp = at->end;
   while (tmp != NULL && tmp->line != pos) {
      tmp = tmp->previous;
   }
   if (tmp != NULL && tmp->ins.op == JMF) {
      tmp->ins.op1 = ln;
      return tmp->line;
   }
   return -1;
}

int jump_while(asmtab *at, unsigned int ret, unsigned int pos, unsigned int ln) {
   asmcell * tmp = at->end;
   if (tmp != NULL && tmp->ins.op == JMP) {
      tmp->ins.op0 = ret;
      while (tmp != NULL && tmp->line != pos) {
         tmp = tmp->previous;
      }
      if (tmp != NULL && tmp->ins.op == JMF) {
         tmp->ins.op1 = ln;
         return tmp->line;
      }
   }
   return -1;
}

void set_main_asm(asmtab *at, unsigned int ln) {
   if (at->size > 0) {
      inst ins = {JMP,ln+1,0,0};
      asmcell* beg = malloc(sizeof(asmcell));
      beg->line = 0;
      beg->previous = NULL;
      beg->ins = ins;
      beg->next = at->begin;
      at->begin = beg;
      at->size++;
      while (beg != NULL && beg->next != NULL) {
         beg = beg->next;
         switch (beg->ins.op) {
            case JMP:
               beg->ins.op0++;
               break;
            case CLL:
               beg->ins.op2++;
            case JMF:
               beg->ins.op1++;
               break;
            default:
               break;
         }
         beg->line++;
      }
   }
}
