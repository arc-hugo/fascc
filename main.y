%{
#include <stdlib.h>
#include <stdio.h>
%}
//%union {int num; char* string;}
%token tMAIN tAO tAF tINT tVOID tIF tWHILE tCONST tEGAL tSOU tADD tMUL tDIV tPO tPF tPV tFL tPRINT tID tNB tEQ tDIF tSUP tINF tSUE tINE tAND tOR
/*
%token <num> tNB tEQ tDIF tSUP tINF tSUE tINE
%token <string> tID
%type <num> Valeur Cond Conds
*/
%start Main
%%
Main : tMAIN tPO tPF Body ; /* Main */
Type : tINT
     | tVOID ;
Body : tAO Insts tAF; /* Corps de fonction/structure de contrôle */
Insts: Inst Insts
     | ;
Inst : Decl
     | Aff
     | Print
     | Ctrl ;
Decl: Type tID tPV /*{ add_sym($1,$2); } /* Déclaration sans affectation */
    | Type tID tEGAL Valeur tPV /*{ add_sym_val($1,$2,$4); }*/ /* Déclaration avec affectation */
    | tCONST Type tID tEGAL Valeur tPV /*{ add_const($2,$3,$5); }*/; /* Déclaration de constante */
Aff: tID tEGAL Valeur tPV /*{ att_sym($1,$3); } /* Attribution */
   | tID tMUL tEGAL Valeur tPV /*{ att_sym($1,$4); } /* Multiplication */
   | tID tDIV tEGAL Valeur tPV /*{ att_sym($1,$4); }*/ /* Division */
   | tID tADD tEGAL Valeur tPV /*{ att_sym($1,$4); } /* Addition */
   | tID tSOU tEGAL Valeur tPV ; /*{ att_sym($1,$4); } /* Soustraction */
Valeur: tNB // { $$ = $1; } /* Nombre */
      | tID //{ get_val($1); } /* Variable */
      | tPO Valeur tPF //{ $$ = $2; } /* Parenthèse */
      | Valeur tADD Valeur //{ $$ = $1 + $3; } /* Addition */
      | Valeur tSOU Valeur //{ $$ = $1 - $3; }/* Soustraction */
      | Valeur tMUL Valeur //{ $$ = $1 * $3; }/* Multiplication */
      | Valeur tDIV Valeur //{ $$ = $1 / $3; }; /* Division */
Print : tPRINT tPO Valeur tPF tPV; //{ printf("%d\n",$3); };
Ctrl  : tIF tPO Conds tPF Body
      | tWHILE tPO Conds tPF Body;
Conds : Conds Sym Conds
      | tPO Conds tPF
      | Cond ;
Sym   : tAND
      | tOR ;
Cond  : Valeur
      | Valeur tEQ Valeur
      | Valeur tDIF Valeur
      | Valeur tSUP Valeur
      | Valeur tSUE Valeur
      | Valeur tINF Valeur
      | Valeur tINE Valeur ;
%%
void yyerror(char *s) { fprintf(stderr, "%s\n", s); exit(1); }
int main(void) {
  yyparse();
  printf("YES\n");
  return 0;
}
