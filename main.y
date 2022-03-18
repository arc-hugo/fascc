%{
#include <stdlib.h>
#include <stdio.h>
#include "symtab.h"
#include "asmtab.h"

unsigned short depth = 0;
symtab * st;
asmtab * at;
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
    | Type tID tEGAL Valeur tPV { add_sym(st,$1,$2,depth); /* ASM */ } /* Déclaration avec affectation */
    | tCONST Type tID tEGAL Valeur tPV /*{ valeur dans le code }*/; /* Déclaration de constante */
Aff: tID tEGAL Valeur tPV { add_asm(at,AFC,get_address(st,$1),$3,0); } /* Attribution */
   | tID tMUL tEGAL Valeur tPV { add_asm(at,MUL,get_address(st,$1),get_address(st,$1),$4);  } /* Multiplication */
   | tID tDIV tEGAL Valeur tPV { add_asm(at,DIV,get_address(st,$1),get_address(st,$1),$4); }*/ /* Division */
   | tID tADD tEGAL Valeur tPV { add_asm(at,ADD,get_address(st,$1),get_address(st,$1),$4); } /* Addition */
   | tID tSOU tEGAL Valeur tPV ; { add_asm(at,SOU,get_address(st,$1),get_address(st,$1),$4); } /* Soustraction */
Valeur: tNB {  } /* Nombre */
      | tID { get_address(st,$1) } /* Variable */
      | tPO Valeur tPF //{ $$ = $2; } /* Parenthèse */
      | Valeur tMUL Valeur { add_asm(at,MUL,$1,$1,$3); $$=$1;  }//{ $$ = $1 * $3; }/* Multiplication */
      | Valeur tDIV Valeur { add_asm(at,DIV,$1,$1,$3); $$=$1;  }//{ $$ = $1 / $3; }; /* Division */
      | Valeur tADD Valeur { add_asm(at,ADD,$1,$1,$3); $$=$1;  } /* Addition */
      | Valeur tSOU Valeur { add_asm(at,SOU,$1,$1,$3); $$=$1;  }//{ $$ = $1 - $3; }/* Soustraction */
Print : tPRINT tPO Valeur tPF tPV; //{ printf("%d\n",$3); };
Ctrl  : tIF tPO Conds tPF Body
      | tWHILE tPO Conds tPF Body;
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
