%{
#include <stdlib.h>
#include <stdio.h>
#include "asmtab.h"
#include "symtab.h"

int yylex();
void yyerror(const char *s);

unsigned short depth = 0;
unsigned short offset = 0;
unsigned short ln = 0;
int add_tmp = 0;
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
%union {int num; char* string; enum type type; enum op op;}
%token tMAIN tAO tAF tINT tVOID tIF tWHILE tCONST tEGAL tSOU tADD tMUL tDIV tPO tPF tPV tFL tPRINT tDEG tDIF tSUP tINF tSUE tINE tAND tOR tVIR
%token <num> tNB
%token <string> tID
%type <num> Valeur Cond Conds
%type <type> Type
%type <op> Sym
%right tEGAL
%left tADD tSOU
%left tMUL tDIV
%start Prg
%%
Prg  : Func Prg
     | Main;
Main : tMAIN tPO tPF Body 
     | tINT tMAIN tPO tPF Body ; /* Main */
Func : Type tID tPO Args tPF Body { printf("fin function"); };  /* Function */
Arg  : Type tID ;
Args : Arg tVIR Args 
     | ;
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
    | Type tID tEGAL Valeur tPV { add_sym(st,$1,$2,depth); offset=0; } /* Déclaration avec affectation */
    | tCONST Type tID tEGAL Valeur tPV /*{ valeur dans le code }*/; /* Déclaration de constante */
Aff: tID tEGAL Valeur tPV { add_asm(at,COP,get_address(st,$1),$3,0); } /* Attribution */
   | tID tMUL tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,MUL,add_tmp,add_tmp,$4); } /* Multiplication */
   | tID tDIV tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,DIV,add_tmp,add_tmp,$4); } /* Division */
   | tID tADD tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,ADD,add_tmp,add_tmp,$4); } /* Addition */
   | tID tSOU tEGAL Valeur tPV { add_tmp = get_address(st,$1); add_asm(at,SOU,add_tmp,add_tmp,$4); } /* Soustraction */;
Valeur: tNB { add_tmp = get_tmp(st,offset++); add_asm(at,AFC,add_tmp,$1,0); $$ = add_tmp;  } /* Nombre */
      | tID { $$ = get_address(st,$1); } /* Variable */
      | tPO Valeur tPF { $$ = $2; } /* Parenthèses */
      | Valeur tMUL Valeur { add_tmp = tmp_add($1,$3); add_asm(at,MUL,add_tmp,$1,$3); $$ = add_tmp; } /* Multiplication */
      | Valeur tDIV Valeur { add_tmp = tmp_add($1,$3); add_asm(at,DIV,add_tmp,$1,$3); $$ = add_tmp; } /* Division */
      | Valeur tADD Valeur { add_tmp = tmp_add($1,$3); add_asm(at,ADD,add_tmp,$1,$3); $$ = add_tmp; } /* Addition */
      | Valeur tSOU Valeur { add_tmp = tmp_add($1,$3); add_asm(at,SOU,add_tmp,$1,$3); $$ = add_tmp; } /* Soustraction */
Print : tPRINT tPO Valeur tPF tPV { add_asm(at,PRI,$3,0,0); offset=0; };
Ctrl  : tIF tPO Conds tPF { add_asm(at,NOT,$3,$3,0); add_asm(at,JMF,$3,0,0); add_asm(at,NOP,0,0,0); offset=0;} Body { jump_nop(at,get_last_line(at)); }
      | tWHILE tPO { ln = get_last_line(at); } Conds tPF { add_asm(at,JMF,$4,0,0); add_asm(at,NOP,0,0,0); offset=0; } Body { add_asm(at,JMP,ln,0,0); jump_nop(at,get_last_line(at)); };
Conds : Conds Sym Conds { add_asm(at,$2,$1,$1,$2); $$ = $1; }
      | tPO Conds tPF { $$ = $2; }
      | Cond { $$ = $1; };
Sym   : tAND { $$ = AND; }
      | tOR { $$ = OR; };
Cond  : Valeur { $$ = $1; }
      | Valeur tDEG Valeur { add_tmp = tmp_add($1,$3); add_asm(at,EQU,add_tmp,$1,$3); $$ = add_tmp; }
      | Valeur tDIF Valeur { add_tmp = tmp_add($1,$3); add_asm(at,EQU,add_tmp,$1,$3); add_asm(at,NOT,add_tmp,add_tmp,0); $$ = add_tmp; }
      | Valeur tSUP Valeur { add_tmp = tmp_add($1,$3); add_asm(at,SUP,add_tmp,$1,$3); $$ = add_tmp; }
      | Valeur tSUE Valeur { add_tmp = tmp_add($1,$3); add_asm(at,INF,add_tmp,$1,$3); add_asm(at,NOT,add_tmp,add_tmp,0); $$ = add_tmp; }
      | Valeur tINF Valeur { add_tmp = tmp_add($1,$3); add_asm(at,INF,add_tmp,$1,$3); $$ = add_tmp; }
      | Valeur tINE Valeur { add_tmp = tmp_add($1,$3); add_asm(at,SUP,add_tmp,$1,$3); add_asm(at,NOT,add_tmp,add_tmp,0); $$ = add_tmp; };
%%
void yyerror(const char *s) { fprintf(stderr, "%s\n", s); exit(1); }
int main(void) {
   st = init_st();
   at = init_at();
   yyparse();
   parse(at);
   printf("YES\n");
   return 0;
}
