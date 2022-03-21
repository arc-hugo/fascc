%{
#include <stdlib.h>
#include <stdio.h>
#include "symtab.h"
#include "asmtab.h"

unsigned short depth = 0;
unsigned short add_tmp = 0;
unsigned short offset = 0;
symtab * st;
asmtab * at;

unsigned short tmp_add(unsigned short left, unsigned short right) {
   unsigned short left_tmp = is_tmp(st,left);
   unsigned short right_tmp = is_tmp(st,right);
   if (left_tmp) {
      if (right_tmp && offset >= 1) {
         offset--;
      }
      return left;
   } else if (right_tmp) {
      return right;
   }
   return get_tmp(st,offset++);
}

%}
%union {int num; char* string; enum type type;}
%token tMAIN tAO tAF tINT tVOID tIF tWHILE tCONST tEGAL tSOU tADD tMUL tDIV tPO tPF tPV tFL tPRINT tID tNB tDEG tDIF tSUP tINF tSUE tINE tAND tOR
%token <num> tNB
%token <string> tID
%type <num> Valeur Cond Conds
%type <type> Type
%start Main
%%
Main : tMAIN tPO tPF Body ; /* Main */
Func : Type tID tPO Args tPF Body {printf("fin function");};  /* Function */ 
Arg : Type tID ; 
Args : Arg tVIR Args ; 
Type : tINT { $$ = INT; }
     | tVOID { $$ = VOID; };
Body : tAO { depth++; } Insts tAF { remove_depth(st,depth); depth--; }; /* Corps de fonction/structure de contrôle */
Insts: Inst Insts
     | ;
Inst : Decl
     | Aff
     | Print
     | Ctrl ;
Decl: Type tID tPV { add_sym(st,$1,$2,depth); } /* Déclaration sans affectation */
    | Type tID tEGAL Valeur tPV { add_tmp = add_sym(st,$1,$2,depth); add_asm(at,COP,add_tmp,$4,0); } /* Déclaration avec affectation */
    | tCONST Type tID tEGAL Valeur tPV /*{ valeur dans le code }*/; /* Déclaration de constante */
Aff: tID tEGAL Valeur tPV { add_asm(at,COP,get_address(st,$1),$3,0); } /* Attribution */
   | tID tMUL tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,MUL,add_tmp,add_tmp,$4);  } /* Multiplication */
   | tID tDIV tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,DIV,add_tmp,add_tmp,$4); }*/ /* Division */
   | tID tADD tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,ADD,add_tmp,add_tmp,$4); } /* Addition */
   | tID tSOU tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,SOU,add_tmp,add_tmp,$4); } /* Soustraction */;
Valeur: tNB { add_tmp = get_tmp(st,offset++); add_asm(at,AFC,add_tmp,$1,0); $$ = add_tmp;  } /* Nombre */
      | tID { $$ = get_address(st,$1); } /* Variable */
      | PO Valeur PF /* Parenthèses */
      | Valeur tMUL Valeur { add_tmp = tmp_add($1,$3); add_asm(at,MUL,add_tmp,$1,$3); $$ = add_tmp; } /* Multiplication */
      | Valeur tDIV Valeur { add_tmp = tmp_add($1,$3); add_asm(at,DIV,add_tmp,$1,$3); $$ = add_tmp; } /* Division */
      | Valeur tADD Valeur { add_tmp = tmp_add($1,$3); add_asm(at,ADD,add_tmp,$1,$3); $$ = add_tmp; } /* Addition */
      | Valeur tSOU Valeur { add_tmp = tmp_add($1,$3); add_asm(at,SOU,add_tmp,$1,$3); $$ = add_tmp; } /* Soustraction */
Print : tPRINT tPO Valeur tPF tPV /* { add_asm(at,PRI,$3,0,0); offset--; } */;
Ctrl  : tIF tPO Conds tPF { add_asm(at,JMF,$3,0,0); add_asm(at,NOP,0,0,0); } Body { jump_if(at,get_last_line(at)); }
      | tWHILE {  } tPO Conds tPF Body { };
Conds : Conds Sym Conds
      | tPO Conds tPF
      | Cond ;
Sym   : tAND
      | tOR ;
Cond  : Valeur
      | Valeur tDEG Valeur
      | Valeur tDIF Valeur
      | Valeur tSUP Valeur
      | Valeur tSUE Valeur
      | Valeur tINF Valeur
      | Valeur tINE Valeur ;
%%
void yyerror(char *s) { fprintf(stderr, "%s\n", s); exit(1); }
int main(void) {
   st = init_st();
   at = init_at();
   yyparse();
   printf("YES\n");
   return 0;
}
