#include <stdlib.h>
#include <stdio.h>

#include "export.h"


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
            fprintf(out,"JMP l%d\n",tmp->ins.op0);
            break;
         case JMF:
            fprintf(out,"JMF @%d l%d\n",tmp->ins.op0,tmp->ins.op1);
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
         case CLL:
            fprintf(out,"CLL %d l%d l%d\n",tmp->ins.op0,tmp->ins.op1, tmp->ins.op2);
            break;
         case RET:
            fprintf(out,"RET\n");
            break;
         default:
            break;
      }
      tmp = tmp->next;
   }
}
