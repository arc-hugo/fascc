%{
#include <stdlib.h>
#include <stdio.h>
#include "asmtab.h"
#include "symtab.h"

int yylex();
void yyerror(const char *s);

unsigned int depth = 0; // Profondeur courante du programme
unsigned int offset = 0; // Décalage courant des variables temporaires
unsigned int ret_add = 0; // Adresse de retour de function
unsigned int base = 0; // Décalage du pointeur de base
unsigned int addr_tmp = 0; // Adresse temporaire utilisée
unsigned int arg_count = 0; // Adresse du prochain argument d'une fonction
symtab * st; // Tableau de symboles
asmtab * at; // Tableau d'instructions

// Fonction de choix de l'adresse temporaire de retour
unsigned int tmp_add(unsigned int left, unsigned int right) {
   unsigned int left_tmp = is_tmp(st,left);
   unsigned int right_tmp = is_tmp(st,right);
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
Func : Type tID tPO Args tPF Body { add_asm(at,RET,0 /* addresse retour */,base,0); };  /* Function */
Args : Arg tVIR Args 
     | Arg
     | ;
Arg  : Type tID ;
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
Aff: tID tEGAL Valeur tPV { add_asm(at,COP,get_address(st,$1),$3,0); reduce_cop(at); offset=0; } /* Attribution */
   | tID tMUL tEGAL Valeur tPV { addr_tmp = get_address(st,$1); add_asm(at,MUL,addr_tmp,addr_tmp,$4); offset=0; } /* Multiplication */
   | tID tDIV tEGAL Valeur tPV { addr_tmp = get_address(st,$1); add_asm(at,DIV,addr_tmp,addr_tmp,$4); offset=0; } /* Division */
   | tID tADD tEGAL Valeur tPV { addr_tmp = get_address(st,$1); add_asm(at,ADD,addr_tmp,addr_tmp,$4); offset=0; } /* Addition */
   | tID tSOU tEGAL Valeur tPV { addr_tmp = get_address(st,$1); add_asm(at,SOU,addr_tmp,addr_tmp,$4); offset=0; } /* Soustraction */;
Valeur: tNB { addr_tmp = get_tmp(st,offset++); add_asm(at,AFC,addr_tmp,$1,0); $$ = addr_tmp; } /* Nombre */
      | tID { $$ = get_address(st,$1); } /* Variable */
      | tPO Valeur tPF { $$ = $2; } /* Parenthèses */
      | Valeur tMUL Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,MUL,addr_tmp,$1,$3); $$ = addr_tmp; } /* Multiplication */
      | Valeur tDIV Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,DIV,addr_tmp,$1,$3); $$ = addr_tmp; } /* Division */
      | Valeur tADD Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,ADD,addr_tmp,$1,$3); $$ = addr_tmp; } /* Addition */
      | Valeur tSOU Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,SOU,addr_tmp,$1,$3); $$ = addr_tmp; }; /* Soustraction */
Print : tPRINT tPO Valeur tPF tPV { add_asm(at,PRI,$3,0,0); offset=0; };
Ctrl  : tIF tPO Conds tPF { add_asm(at,JMF,$3,0,0); add_asm(at,NOP,0,0,0); offset=0;} Body { jump_nop(at,get_last_line(at)); }
      | tWHILE tPO { add_asm(at,CND,0,0,0); } Conds tPF { add_asm(at,JMF,$4,0,0); add_asm(at,NOP,0,0,0); offset=0; } Body { add_asm(at,JMP,0,0,0); jump_nop(at,get_last_line(at)); jump_cnd(at); };
Conds : Conds Sym Conds { addr_tmp = tmp_add($1,$3); add_asm(at,$2,addr_tmp,$1,$3); $$ = addr_tmp; }
      | tPO Conds tPF { $$ = $2; }
      | Cond { $$ = $1; };
Sym   : tAND { $$ = AND; }
      | tOR { $$ = OR; };
Cond  : Valeur { $$ = $1; }
      | Valeur tDEG Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,EQU,addr_tmp,$1,$3); $$ = addr_tmp; }
      | Valeur tDIF Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,EQU,addr_tmp,$1,$3); add_asm(at,NOT,addr_tmp,addr_tmp,0); $$ = addr_tmp; }
      | Valeur tSUP Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,SUP,addr_tmp,$1,$3); $$ = addr_tmp; }
      | Valeur tSUE Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,INF,addr_tmp,$1,$3); add_asm(at,NOT,addr_tmp,addr_tmp,0); $$ = addr_tmp; }
      | Valeur tINF Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,INF,addr_tmp,$1,$3); $$ = addr_tmp; }
      | Valeur tINE Valeur { addr_tmp = tmp_add($1,$3); add_asm(at,SUP,addr_tmp,$1,$3); add_asm(at,NOT,addr_tmp,addr_tmp,0); $$ = addr_tmp; };
%%
void yyerror(const char *s) { fprintf(stderr, "%s\n", s); exit(1); }
int main(int argc, char** argv) {
   st = init_st();
   at = init_at();
   yyparse();
   FILE* out = fopen("./out","w");
   export_asm(at,out);
   printf("YES\n");
   return 0;
}
