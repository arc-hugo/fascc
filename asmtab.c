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
   } else {
      at->end->next = malloc(sizeof(asmcell));
      at->end->next->previous = at->end;
      at->end = at->end->next;
      at->end->ins = ins;
   }
   at->size++;
   return 0;
}

int remove_nop(asmtab * at, asmcell * c) {
   asmcell * tmp = at->end;
   while (tmp != NULL && tmp->ins.op != NOP) {
      tmp = tmp->previous;
   }
   if (tmp != NULL) {
      tmp->previous->next = tmp->next;
      c->next = tmp->next;
      c->previous = tmp->previous;
      c->ins = tmp->ins;
      free(tmp);
      return 0;
   }
   return -1;
}

int add_asm(asmtab * at, enum op op, unsigned short op0, unsigned short op1, unsigned short op2) {
   inst ins = { op, op0, op1, op2 };
   return add(at, ins);
}

int jump_if(asmtab *at, unsigned int ln) {
   asmcell cell;
   int ret = remove_nop(at, &cell);
   if (ret == 0) {
      cell.previous->ins.op1 = ln;
   }
   return ret;
}

void parse(asmtab * at) {
   unsigned int line = 0;
   asmcell * tmp = at->begin;
   while (tmp != NULL) {
      printf("%d ",line++);
      switch (tmp->ins.op) {
         case ADD:
            printf("ADD @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case MUL:
            printf("MUL @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case SOU:
            printf("SOU @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case DIV:
            printf("DIV @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case COP:
            printf("COP @%d @%d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case AFC:
            printf("AFC @%d %d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case JMP:
            printf("JMP %d\n",tmp->ins.op0);
            break;
         case JMF:
            printf("JMF @%d %d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case INF:
            printf("INF @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case SUP:
            printf("SUP @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case EQU:
            printf("EQU @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case PRI:
            printf("PRI @%d\n",tmp->ins.op0);
            break;
         default:
            break;
      }
      tmp = tmp->next;
   }
}
