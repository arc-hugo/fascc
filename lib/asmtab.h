#ifndef ASMTAB_H__
#define ASMTAB_H__
#include <stdio.h>

enum op {
   ADD = 0,
   SOU = 1,
   MUL = 2,
   DIV = 3,
   AND = 4,
   OR = 5,
   NOT = 6,
   COP = 7,
   AFC = 8,
   JMP = 9,
   JMF = 10,
   INF = 11,
   SUP = 12,
   EQU = 13,
   PRI = 14,
   NOP,
   CND
};

typedef struct inst {
   enum op op;
   unsigned int op0;
   unsigned int op1;
   unsigned int op2;
}inst;

struct asmcell;
typedef struct asmcell {
   inst ins;
   unsigned int line;
   struct asmcell* next;
   struct asmcell* previous;
} asmcell;

typedef struct {
   unsigned int size;
   asmcell* begin;
   asmcell* end;
} asmtab;

asmtab * init_at();
unsigned int get_last_line(asmtab * at);
int add_asm(asmtab * at, enum op op, unsigned int op0, unsigned int op1, unsigned int op2);
int reduce_cop(asmtab * at);
int jump_nop(asmtab * at, unsigned int ln);
int jump_cnd(asmtab * at);
void execute(asmtab * at, unsigned int* data, unsigned int max);
void export_asm(asmtab * at, FILE* out);

#endif
