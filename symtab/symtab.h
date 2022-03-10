#ifndef SYMTAB_H__
#define SYMTAB_H__

typedef enum {
   INT,
   VOID
}type;

typedef struct {
   char* name;
   unsigned short add;
   type t;
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
