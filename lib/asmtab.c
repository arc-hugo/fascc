#include <stdlib.h>
#include <stdio.h>
#include "asmtab.h"

asmtab * init_at() {
   asmtab * at = malloc(sizeof(asmtab));
   at->size = 0;
   add_asm(at, SRT, 0, 0, 0);
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
         at->end->previous->ins.op == AFC &&
         at->end->previous->ins.op0 == at->end->ins.op1) {
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
         } else if (tmp->ins.op == JMP && tmp->ins.op0 > ret) {
            tmp->ins.op0--;
         }
         tmp->line--;
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
      at->end->ins.op0 = ret;
      while (tmp != at->end) {
         tmp = tmp->next;
         if (tmp->ins.op == JMP && tmp->ins.op0 > ret) {
            tmp->ins.op0--;
         } else if (tmp->ins.op == JMF) {
            tmp->ins.op1--;
         }
         tmp->line--;
      }
      free(*add);
   }
   free(add);
   return ret;
}

asmcell* jump(asmcell * c, unsigned int pc, unsigned int add) {
   while (c != NULL && pc < add) {
      c = c->next;
      pc++;
   }
   while (c != NULL && pc > add) {
      c = c->previous;
      pc--;
   }
   return c;
}

void execute(asmtab * at, unsigned int* data, unsigned int max) {
   asmcell * pp = at->begin;
   unsigned int pc = 0;
   unsigned int add = 0;
   while (pp != NULL) {
      switch (pp->ins.op) {
         case ADD:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) + *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case MUL:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) * *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case SOU:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) - *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case DIV:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) / *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case COP:
            *(data+pp->ins.op0) = *(data+pp->ins.op1);
            pp = pp->next;
            pc++;
            break;
         case AFC:
            *(data+pp->ins.op0) = pp->ins.op1;
            pp = pp->next;
            pc++;
            break;
         case JMP:
            add = pp->ins.op0;
            pp = jump(pp, pc, add);
            pc = add;
            break;
         case JMF:
            if (*(data+pp->ins.op0) == 0) {
               add = pp->ins.op1;
               pp = jump(pp, pc, add);
               pc = add;
            } else {
               pp = pp->next;
               pc++;
            }
            break;
         case INF:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) < *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case SUP:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) > *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case EQU:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) == *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case PRI:
            printf("%d\n",*(data+pp->ins.op0));
            pp = pp->next;
            pc++;
            break;
         case AND:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) && *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case OR:
            *(data+pp->ins.op0) = *(data+pp->ins.op1) || *(data+pp->ins.op2);
            pp = pp->next;
            pc++;
            break;
         case NOT:
            *(data+pp->ins.op0) = !(*(data+pp->ins.op1));
            pp = pp->next;
            pc++;
            break;
         default:
            pp = pp->next;
            pc++;
            break;
      }
   }
}

void export_asm(asmtab * at, FILE* out) {
   asmcell * tmp = at->begin;
   while (tmp != NULL) {
      switch (tmp->ins.op) {
         case ADD:
            fprintf(out,"ADD @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case MUL:
            fprintf(out,"MUL @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case SOU:
            fprintf(out,"SOU @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case DIV:
            fprintf(out,"DIV @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case COP:
            fprintf(out,"COP @%d @%d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case AFC:
            fprintf(out,"AFC @%d %d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case JMP:
            fprintf(out,"JMP %d\n",tmp->ins.op0);
            break;
         case JMF:
            fprintf(out,"JMF @%d %d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         case INF:
            fprintf(out,"INF @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case SUP:
            fprintf(out,"SUP @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case EQU:
            fprintf(out,"EQU @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case PRI:
            fprintf(out,"PRI @%d\n",tmp->ins.op0);
            break;
         case AND:
            fprintf(out,"AND @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case OR:
            fprintf(out,"OR @%d @%d @%d\n",tmp->ins.op0,tmp->ins.op1,tmp->ins.op2);
            break;
         case NOT:
            fprintf(out,"NOT @%d @%d\n",tmp->ins.op0,tmp->ins.op1);
            break;
         default:
            break;
      }
      tmp = tmp->next;
   }
}
