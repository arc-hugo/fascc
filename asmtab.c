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

int get_line(asmtab *at, asmcell* c) {
   asmcell* tmp = at->begin;
   int count = 0;
   while (tmp != NULL && tmp != c) {
      count++;
      tmp = tmp->next;
   }
   if (tmp == NULL) {
      return -1;
   }
   return count;
}

int add(asmtab * at, inst ins) {
   if (get_last_line(at) == 0) {
      at->begin = malloc(sizeof(asmcell));
      at->end = at->begin;
      at->begin->ins = ins;
      at->begin->previous = NULL;
      at->begin->next = NULL;
   } else {
      at->end->next = malloc(sizeof(asmcell));
      at->end->next->previous = at->end;
      at->end = at->end->next;
      at->end->next = NULL;
      at->end->ins = ins;
   }
   at->size++;
   return 0;
}

int remove_op(asmtab * at, enum op op, asmcell ** c) {
   asmcell * tmp = at->end;
   int line = 0;
   while (tmp != NULL && tmp->ins.op != op) {
      tmp = tmp->previous;
      line++;
   }
   if (tmp != NULL) {
      tmp->previous->next = tmp->next;
      tmp->next->previous = tmp->previous;
      *c = tmp;
      at->size--;
      return (at->size+1)-line;
   }
   return -1;
}

int add_asm(asmtab * at, enum op op, unsigned short op0, unsigned short op1, unsigned short op2) {
   inst ins = { op, op0, op1, op2 };
   return add(at, ins);
}

int jump_nop(asmtab *at, unsigned int ln) {
   asmcell** add = malloc(sizeof(asmcell*));
   int ret = remove_op(at, NOP, add);
   if (ret >= 0 && (*add)->previous != NULL) {
      asmcell* tmp = *add;
      tmp->previous->ins.op1 = ln-1;
      while (ret >= 0 && tmp != NULL && tmp != at->end) {
         tmp = tmp->next;
         if (tmp->ins.op == JMF) {
            tmp->ins.op1--;
         }
         if (tmp->ins.op == JMP) {
            tmp->ins.op0--;
         }
      }
      free(*add);
   }
   free(add);
   return ret;
}

int jump_cnd(asmtab *at) {
   asmcell** add = malloc(sizeof(asmcell*));
   int ret = remove_op(at, CND, add);
   if (ret >= 0 && (*add)->next != NULL) {
      asmcell* tmp = *add;
      printf("%d\n",ret);
      at->end->ins.op0 = ret;
      while (tmp != at->end) {
         tmp = tmp->next;
         if (tmp->ins.op == JMP) {
            tmp->ins.op0--;
         } else if (tmp->ins.op == JMF) {
            tmp->ins.op1--;
         }
      }
      free(*add);
   }
   free(add);
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
         case AND:
            printf("AND @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case OR:
            printf("OR @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case NOT:
            printf("NOT @%d @%d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         default:
            break;
      }
      tmp = tmp->next;
   }
}
