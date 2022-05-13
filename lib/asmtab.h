#ifndef ASMTAB_H__
#define ASMTAB_H__
#include <stdio.h>

enum op {
   NOP = 0,
   ADD = 1,
   SOU = 2,
   MUL = 3,
   DIV = 4,
   AND = 5,
   OR = 6,
   NOT = 7,
   COP = 8,
   AFC = 9,
   JMP = 10,
   JMF = 11,
   INF = 12,
   SUP = 13,
   EQU = 14,
   PRI = 15,
   CND = 16
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
