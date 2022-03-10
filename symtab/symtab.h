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

#endif
