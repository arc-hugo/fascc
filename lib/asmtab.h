#ifndef ASMTAB_H__
#define ASMTAB_H__

enum op {
   ADD = 1,
   MUL,
   SOU,
   DIV,
   COP,
   AFC,
   JMP,
   JMF,
   INF,
   SUP,
   EQU,
   PRI,
   AND,
   OR,
   NOT,
   NOP,
   CND
};

typedef struct inst {
   enum op op;
   unsigned short op0;
   unsigned short op1;
   unsigned short op2;
}inst;

struct asmcell;
typedef struct asmcell {
   inst ins;
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
int add_asm(asmtab * at, enum op op, unsigned short op0, unsigned short op1, unsigned short op2);
int reduce_cop(asmtab * at);
int jump_nop(asmtab * at, unsigned int ln);
int jump_cnd(asmtab * at);
void parse(asmtab * at);

#endif
