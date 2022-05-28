%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "type.h"
#include "function.h"
#include "asmtab.h"
#include "condtab.h"
#include "funtab.h"
#include "symtab.h"
#include "export.h"

// Ligne courante
extern int yylineno;
// Fichier à parser
extern FILE* yyin;

int yylex();
void yyerror(const char *s);

unsigned int depth = 0; // Profondeur courante du programme
unsigned int offset = 0; // Décalage des variables temporaires
unsigned int fun_offset = 0; // Décalage des arguments de fonction
unsigned int ret_add = 0; // Adresse de retour de function
unsigned int addr_tmp = 0; // Adresse temporaire utilisée
unsigned int arg_count = 0; // Adresse du prochain argument d'une fonction
unsigned short erreur = 0; // Détection d'une erreur
unsigned short cst = 0; // Valeur constante

function* fun; // Création de fonction
function* call;// Appel de fonction

symtab * st; // Tableau de symboles
condtab * ct; // Pile de structure de contrôle
funtab * ft; // Tableau de fonctions
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

%define parse.error verbose
%define parse.lac full

%union {int num; char* string; enum type type; enum op op;}

%token tAO tAF tINT tVOID tIF tELSE tWHILE tCONST tPV tFL tPRINT tVIR tRET
%token <num> tNB
%token <string> tID tMAIN

%type <num> Valeur
%type <type> Type MType

%right tEGAL
%left tOR
%left tAND
%left tDEG tDIF
%left tINF tINE tSUE tSUP
%left tADD tSOU
%left tMUL tDIV
%left tPO tPF

%nonassoc tTHEN
%nonassoc tELSE

%start Prg
%%
Prg  : Func Prg
     | Main { YYACCEPT; }
     /* Forme d'un programme */;
Main : MType tMAIN { 
         // Indication du début de programme en ASM
         set_main_asm(at,get_last_line(at)); 
         // Ajout de la fonction main à la liste des fonctions 
         set_main_fun(ft,get_last_line(at),$1);
     }
     tPO tPF Body 
     /* Fonction pricipale */;
MType: Type     
     | { $$ = INT; }
     /* Type possible pour la fonction principale */;
Func : Type tID { 
         // Définition de la nouvelle fonction
         fun = init_fun($2,get_last_line(at),$1); 
     } tPO DArgList tPF { 
         // Ajout à la liste des fonctions
         add_fun(ft,fun); 
     } Body { 
         // Ajout de l'instruction de retour
         add_asm(at,RET,0,0,0); 
         offset=0;
         //TODO reduce return 
     }
      /* Fonction avec arguments */;
DArgList : | DArgs;
DArgs: DArg tVIR DArgs
     | DArg
     /* Déclaration d'arguments */;
DArg : Type tID {
         // Ajout d'un argument à la liste
         add_arg(fun,$2,$1);
         add_sym(st,$1,$2,1);
     } /* Argument déclaré */;
Type : tINT { $$ = INT; }
     | tVOID { $$ = VOID; }
     /* Type de variables/arguments/fonctions */;
Body : tAO {
         // Augmentation de la profondeur
         depth++; 
     } Insts tAF {
         // Diminution de la profondeur et suppression des anciennes variables
         remove_depth(st,depth); 
         depth--;
     } /* Corps de fonction/structure de contrôle */;
Insts: Inst Insts
     | /* Séquence d'instructions */;
Inst : Decl tPV
     | Aff tPV
     | Call tPV
     | Return tPV
     | Print tPV
     | Ctrl 
     | error tPV 
     | error tAF 
     /* Types d'instruction */;
Decl : Type tID { 
         add_sym(st,$1,$2,depth); 
     } /* Déclaration sans affectation */
     | Type tID tEGAL Valeur { 
         add_sym(st,$1,$2,depth);
         offset=0;
     } /* Déclaration avec affectation */
     | tCONST Type tID {
         add_cst(st,$2,$3,depth);
     } /* Constante sans affectation */
     | tCONST Type tID tEGAL Valeur {
         add_cst(st,$2,$3,depth);
         offset=0;   
     } /* Constante avec affectation */
     ;
Aff: tID tEGAL Valeur {
      add_asm(at,COP,get_sym_address(st,$1,&cst),$3,0);
      if (cst == 1)
         yyerror("impossible d'assigner une valeur à la constante");
      reduce_cop(at);
      offset=0;
   } /* Attribution */
   | tID tMUL tEGAL Valeur { 
      addr_tmp = get_sym_address(st,$1,&cst); 
      if (cst == 1)
         yyerror("impossible d'assigner une valeur à la constante");
      add_asm(at,MUL,addr_tmp,addr_tmp,$4); 
      offset=0; 
   } /* Multiplication */
   | tID tDIV tEGAL Valeur { 
      addr_tmp = get_sym_address(st,$1,&cst); 
      if (cst == 1)
         yyerror("impossible d'assigner une valeur à la constante");
      add_asm(at,DIV,addr_tmp,addr_tmp,$4); 
      offset=0; 
   } /* Division */
   | tID tADD tEGAL Valeur { 
      addr_tmp = get_sym_address(st,$1,&cst); 
      if (cst == 1)
         yyerror("impossible d'assigner une valeur à la constante");
      add_asm(at,ADD,addr_tmp,addr_tmp,$4); 
      offset=0; 
   } /* Addition */
   | tID tSOU tEGAL Valeur {
      addr_tmp = get_sym_address(st,$1,&cst); 
      if (cst == 1)
         yyerror("impossible d'assigner une valeur à la constante");
      add_asm(at,SOU,addr_tmp,addr_tmp,$4); 
      offset=0; 
   } /* Soustraction */;
Valeur: tNB { 
         addr_tmp = get_tmp(st,offset++); 
         add_asm(at,AFC,addr_tmp,$1,0); 
         $$ = addr_tmp; 
      } /* Nombre */
      | tID { 
         $$ = get_sym_address(st,$1,&cst);
      } /* Variable */
      | Call { 
         if (call->t == VOID)
            yyerror("valeur void n'a pas pû être ignorée");
         addr_tmp = get_tmp(st,offset++);
         add_asm(at,COP,addr_tmp,get_tmp(st,offset+1),0);
         $$ = addr_tmp;
      } /* Retour de fonction */
      | tPO Valeur tPF { $$ = $2; } /* Parenthèses */
      | Valeur tMUL Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,MUL,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tDIV Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,DIV,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tADD Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,ADD,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tSOU Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,SOU,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tDEG Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,EQU,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tDIF Valeur {
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,EQU,addr_tmp,$1,$3); 
         add_asm(at,NOT,addr_tmp,addr_tmp,0); 
         $$ = addr_tmp;  
      }
      | Valeur tSUP Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,SUP,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tSUE Valeur {
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,INF,addr_tmp,$1,$3); 
         add_asm(at,NOT,addr_tmp,addr_tmp,0); 
         $$ = addr_tmp;  
      }
      | Valeur tINF Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,INF,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tINE Valeur {
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,SUP,addr_tmp,$1,$3); 
         add_asm(at,NOT,addr_tmp,addr_tmp,0); 
         $$ = addr_tmp;  
      }
      | Valeur tAND Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,AND,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      | Valeur tOR Valeur { 
         addr_tmp = tmp_add($1,$3); 
         add_asm(at,OR,addr_tmp,$1,$3); 
         $$ = addr_tmp; 
      }
      ;
Call  : tID tPO { 
         // Initialisation du décalage des paramètres
         fun_offset=offset; 
      } Args tPF {
         // Récupération de la fonction
         ret_add = get_fun(ft,$1,call);
         if (ret_add < 0)
            yyerror("fonction indéfinie");
         if (arg_count != call->argc)
            yyerror("nombre d'arguments invalide");
         // Instruction d'appel de fonction (décalage base, adresse de retour et saut)
         add_asm(at,CLL,get_tmp(st,offset),get_last_line(at)+1,call->add);
         arg_count=0;
         fun_offset=0;
      } /* Appel de fonction avec arguments */
      | tID tPO tPF {
         // Récupération de la fonction
         ret_add = get_fun(ft,$1,call);
         if (ret_add < 0) 
            yyerror("fonction indéfinie");
         if (call->argc != 0)
            yyerror("nombre d'arguments invalide");
         // Instruction d'appel de fonction (décalage base, adresse de retour et saut)
         add_asm(at,CLL,get_tmp(st,offset),get_last_line(at)+1,call->add);
      } /* Appel de fonction sans arguments */;
Args  : Arg tVIR Args
      | Arg
      /* Liste d'arguments */;
Arg   : Valeur {
         // Ajustement de l'offset si la valeur est temporaire
         if (is_tmp(st,$1)) {
            offset--;
         }
         // Ajout de l'argument de fonction dans la mémoire temporaire
         add_asm(at,COP,get_tmp(st,fun_offset+2),$1,0);
         //TODO reduce_cop
         arg_count++;
         fun_offset++;
      } /* Valeur en argument */;
Return: tRET Valeur {
         if (fun->t == VOID)
            yyerror("retour de valeur impossible dans une fonction void");
         add_asm(at,COP,0,$2,0);
         reduce_cop(at);
         add_asm(at,RET,0,0,0);
      } /* Valeur en retour de fonction */
      | tRET {
         // Retour de fonction (décalage base et saut vers l'adresse de retour)
         if (fun->t != VOID) 
            yyerror("retour sans valeur impossible dans une fonction non-void");
         add_asm(at,RET,0,0,0);
      } /* Retour de fonction sans valeur */;
Print : tPRINT tPO Valeur tPF {
         add_asm(at,PRI,$3,0,0);
         offset=0;
      } /* Instruction d'affichage */;
Ctrl  : tIF tPO Valeur tPF {
         // Saut conditionel hors du corps du IF
         push_cond(ct,get_last_line(at));
         add_asm(at,JMF,$3,0,0);
         offset=0;
      } Body IfN
      | tWHILE tPO {
         // Début de la suite de conditions du WHILE
         push_cond(ct,get_last_line(at));
      } Valeur tPF {
         // Saut conditionel hors du corps du WHILE
         push_cond(ct,get_last_line(at));
         add_asm(at,JMF,$4,0,0); 
         offset=0;
      } Body {
         // Saut au début des conditions
         add_asm(at,JMP,0,0,0);
         // Définition de la ligne de saut de sortie et de la ligne de retour aux conditions
         jump_while(at,pop_cond(ct),pop_cond(ct),get_last_line(at));
      }
      /* Structures de contrôle */;
IfN   : %prec tTHEN {
         // Définition de la ligne de saut
         jump_if(at,pop_cond(ct),get_last_line(at));
      }
      | tELSE {
         // Définition de la ligne de saut
         jump_if(at,pop_cond(ct),get_last_line(at)+1);
         // Saut hors du corps du ELSE
         push_cond(ct,get_last_line(at));
         add_asm(at,JMP,0,0,0);
      } Body {
         // Définition de la ligne de saut
         jump_else(at,pop_cond(ct),get_last_line(at)); 
      };
%%

// Gestion des erreurs
void yyerror(const char *s) {
   fprintf(stderr, "\e[1m%d:\e[m %s\n", yylineno, s);
   erreur = 1;
}

// Fonction principale de compilation
int main(int argc, char** argv) {
   // Fichier de sorti personnalisé
   char* ov = "out";
   // Variables de getopt
   int c;
   opterr = 0;

   // Parcours des arguments optionnels
   while((c = getopt(argc,argv,"o:")) != -1) {
      switch(c) {
         case 'o':
            ov = strdup(optarg);
            break;
         default:
            break;
      }
   }
   
   // Spécification du fichier à compiler
   if (optind < argc) {
      yyin = fopen(argv[optind],"r");
      // Ouverture du fichier à compiler
      if (yyin) {
         st = init_st();
         ct = init_ct();
         ft = init_ft();
         at = init_at();
         call = malloc(sizeof(function));
         yyparse();
         // Pas d'erreur lors de la compilation
         if (!erreur) {
            FILE* out = fopen(ov,"w");
            // Ouverture du ficher de sortie
            if (out) {
               export_asm(at,out);
               return 0;
            } 
            fprintf(stderr,"Erreur d'écriture du fichier %s\n",ov);
            return 4;
         }
         return 3;
      }
      fprintf(stderr,"Erreur d'ouverture du fichier %s\n",argv[optind]);
      return 2;
   }
   fprintf(stderr,"Fichier à compiler non spécifié\n");
   return 1;
}
