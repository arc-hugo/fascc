#ifndef SYMTAB_H__
#define SYMTAB_H__

#include "type.h"

typedef struct {
   char* name;
   unsigned int add;
   enum type t;
   unsigned short depth;
   unsigned short cst;
} variable;

struct symcell;
typedef struct symcell {
   variable var;
   struct symcell* next;
} symcell;

typedef struct {
   unsigned int height;
   symcell* begin;
} symtab;

symtab * init_st();
int height_st(symtab * st);

int add_sym(symtab * st, enum type t, char* name, unsigned short depth);
int add_cst(symtab *st, enum type t, char *name, unsigned short depth);

int get_sym_address(symtab * st, char* name, unsigned short * cst);
unsigned short get_tmp(symtab * st, unsigned short offset);

int remove_depth(symtab * st, unsigned short depth);

unsigned short is_tmp(symtab * st, unsigned short add);

#endif
