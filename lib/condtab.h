#ifndef CONDTAB_H__
#define CONDTAB_H__

struct condcell {
   unsigned int ln;
   struct condcell * next;
};
typedef struct condcell condcell;

typedef struct {
   unsigned int height;
   condcell * beg;
} condtab;

condtab * init_ct();
unsigned int push_cond(condtab * ct, unsigned int ln);
unsigned int pop_cond(condtab * ct);

#endif
