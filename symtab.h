#ifndef SYMTAB_H__
#define SYMTAB_H__

enum type {
   INT,
   VOID
};

typedef struct {
   char* name;
   unsigned short add;
   enum type t;
   unsigned short depth;
} variable;

struct cell;
typedef struct cell {
   variable var;
   struct cell* next;
} cell;

typedef struct {
   unsigned int height;
   cell* begin;
} symtab;

symtab * init_stack();

int get_address(symtab * st, char* name);

int add_sym(symtab * st, enum type t, char* name, unsigned short depth);

int remove_depth(symtab * st, unsigned short depth);

#endif
