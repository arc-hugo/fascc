#include <stdlib.h>
#include <stdio.h>
#include "execute.h"


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
   unsigned int bp = 0;
   while (pp != NULL) {
      printf("\e[1m%d:\e[m ",pc);
      switch (pp->ins.op) {
         case ADD:
            printf("@%d <- @%d (%d) + @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) + *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case MUL:
            printf("@%d <- @%d (%d) * @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) * *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case SOU:
            printf("@%d <- @%d (%d) - @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) - *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case DIV:
            printf("@%d <- @%d (%d) / @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) / *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case COP:
            printf("@%d <- @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp);
            pp = pp->next;
            pc++;
            break;
         case AFC:
            printf("@%d <- %d",pp->ins.op0+bp,pp->ins.op1);
            *(data+pp->ins.op0+bp) = pp->ins.op1;
            pp = pp->next;
            pc++;
            break;
         case JMP:
            printf("jump to l%d",pp->ins.op0+bp);
            add = pp->ins.op0;
            pp = jump(pp, pc, add);
            pc = add;
            break;
         case JMF:
            if (*(data+pp->ins.op0+bp) == 0) {
               printf("false, jump to l%d",pp->ins.op1+bp);
               add = pp->ins.op1;
               pp = jump(pp, pc, add);
               pc = add;
            } else {
               printf("no jump to l%d",pp->ins.op1+bp);
               pp = pp->next;
               pc++;
            }
            break;
         case INF:
            printf("@%d <- @%d (%d) < @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) < *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case SUP:
            printf("@%d <- @%d (%d) > @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) > *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case EQU:
            printf("@%d <- @%d (%d) == @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) == *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case PRI:
            printf("printing @%d -> %d",pp->ins.op0+bp,*(data+pp->ins.op0+bp));
            pp = pp->next;
            pc++;
            break;
         case AND:
            printf("@%d <- @%d (%d) && @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) && *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case OR:
            printf("@%d <- @%d (%d) || @%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp),pp->ins.op2+bp,*(data+pp->ins.op2+bp));
            *(data+pp->ins.op0+bp) = *(data+pp->ins.op1+bp) || *(data+pp->ins.op2+bp);
            pp = pp->next;
            pc++;
            break;
         case NOT:
            printf("@%d <- !@%d (%d)",pp->ins.op0+bp,pp->ins.op1+bp,*(data+pp->ins.op1+bp));
            *(data+pp->ins.op0+bp) = !(*(data+pp->ins.op1+bp));
            pp = pp->next;
            pc++;
            break;
         case CLL:
            *(data+pp->ins.op0+bp) = pp->ins.op0+2;
            *(data+pp->ins.op0+bp+1) = pp->ins.op1;
            bp += pp->ins.op0+2;
            add = pp->ins.op2;
            printf("call fun at l%d, new bp %d",add,bp);
            pp = jump(pp, pc, add);
            pc = add;
            break;
         case RET:
            bp -= 2;
            add = *(data+bp+1);
            pp = jump(pp, pc, add);
            pc = add;
            bp -= *(data+bp)-2;
            printf("return to l%d, new bp %d",add,bp);
            break;
         default:
            pp = pp->next;
            pc++;
            break;
      }
      printf("\n");
   }
}
