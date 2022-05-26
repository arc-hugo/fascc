#include <stdlib.h>

#include "condtab.h"

condtab * init_ct() {
   condtab * ct = malloc(sizeof(condtab));
   ct->height = 0;
   ct->beg = NULL;
   return ct;
}

unsigned int push_cond(condtab * ct, unsigned int ln) {
   condcell * cc = malloc(sizeof(condcell));
   cc->ln = ln;
   cc->next = ct->beg;
   ct->beg = cc;
   return ct->height++;
}

unsigned int pop_cond(condtab * ct) {
   condcell * cc = ct->beg;
   unsigned int ln = cc->ln;
   ct->beg = cc->next;
   free(cc);
   return ln;
}
