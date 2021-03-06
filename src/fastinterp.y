%{
#include <stdlib.h>
#include <stdio.h>
#include "asmtab.h"
#include "execute.h"

#define MAX_DATA 4096

extern FILE* yyin;

int yylex();
void yyerror(const char *s);

unsigned int data[MAX_DATA];
unsigned short erreur = 0;
asmtab * at;

%}
%union {int num;}
%token tADD tMUL tSOU tDIV tCOP tAFC tJMP tJMF tINF tSUP tEQU tPRI tAND tOR tNOT tCLL tRET
%token <num> tNB tADDR tLINE
%start Insts
%%
Insts : Inst Insts
      | ;
Inst  : tADD tADDR tADDR tADDR { add_asm(at,ADD,$2,$3,$4); }
      | tMUL tADDR tADDR tADDR { add_asm(at,MUL,$2,$3,$4); }
      | tSOU tADDR tADDR tADDR { add_asm(at,SOU,$2,$3,$4); }
      | tDIV tADDR tADDR tADDR { add_asm(at,DIV,$2,$3,$4); }
      | tCOP tADDR tADDR { add_asm(at,COP,$2,$3,0); }
      | tAFC tADDR tNB { add_asm(at,AFC,$2,$3,0); }
      | tJMP tLINE { add_asm(at,JMP,$2,0,0); }
      | tJMF tADDR tLINE { add_asm(at,JMF,$2,$3,0); }
      | tINF tADDR tADDR tADDR { add_asm(at,INF,$2,$3,$4); }
      | tSUP tADDR tADDR tADDR { add_asm(at,SUP,$2,$3,$4); }
      | tEQU tADDR tADDR tADDR { add_asm(at,EQU,$2,$3,$4); }
      | tPRI tADDR { add_asm(at,PRI,$2,0,0); }
      | tAND tADDR tADDR tADDR { add_asm(at,AND,$2,$3,$4); }
      | tOR tADDR tADDR tADDR { add_asm(at,OR,$2,$3,$4); }
      | tNOT tADDR tADDR { add_asm(at,NOT,$2,$3,0); }
      | tCLL tNB tLINE tLINE { add_asm(at,CLL,$2,$3,$4); }
      | tRET { add_asm(at,RET,0,0,0); }
      | error '\n' { yyerror(""); }
      ;
%%

void yyerror(const char *s) { 
   fprintf(stderr, "%s\n", s);
   erreur = 1;
}

int main(int argc, char** argv) {
   if (argc > 1) {
      yyin = fopen(argv[1],"r");
      if (yyin) {
         at = init_at();
         yyparse();
         if (!erreur) {
            execute(at,data,MAX_DATA);
            return 0;
         }
      }
      fprintf(stderr,"Erreur d'ouverture du fichier %s",argv[0]);
      return 2;
   }
   fprintf(stderr,"Fichier assembleur non spécifié");
   return 1;
}
