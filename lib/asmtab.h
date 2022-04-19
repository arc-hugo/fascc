#ifndef ASMTAB_H__
#define ASMTAB_H__
#include <stdio.h>

enum op {
   ADD = 1, // Addition des valeurs de deux adresses vers une autre
   MUL, // Multiplication des valeurs de deux adresses vers une autre
   SOU, // Soustraction des valeurs de deux adresses vers une autre
   DIV, // Division deq valeurs de deux adresses vers une autre
   COP, // Copie la valeur d'une adresse vers une autre adresse
   AFC, // Affecte une valeur à une adresse
   JMP, // Saut inconditionnel vers une instruction
   JMF, // Saut vers une instruction si condition dans adresse est fausse
   INF, // Comparaison inférieur des valeurs de deux adresses vers une autre
   SUP, // Comparaison supérieur des valeurs de deux adresses vers une autre
   EQU, // Comparaison égale de deux valeurs de deux adresses vers une adresse
   PRI, // Affichage de la valeur d'une adresse
   AND, // ET logique entre deux valeurs vers une adresse
   OR, // OU logique entre deux valeurs vers une adresse
   NOT, // NON logique d'une valeurs vers une adresse
   CLL, // Sauvegarde de l'adresse de retour et décalage du base pointer lors d'un appel de fonction
   PSH, // Pousse l'argument d'une fonction
   RET, // Décalage du base pointer et sortie vers l'adresse de retour de la fonction
   NOP, // Instruction vide
   CND // Instruction vide précédent des conditions
};

typedef struct inst {
   enum op op;
   unsigned int op0;
   unsigned int op1;
   unsigned int op2;
}inst;

struct asmcell;
typedef struct asmcell {
   inst ins;
   unsigned int line;
   struct asmcell* next;
   struct asmcell* previous;
} asmcell;

typedef struct {
   unsigned int size;
   asmcell* begin;
   asmcell* end;
} asmtab;

asmtab * init_at();
unsigned int get_last_line(asmtab * at);
int add_asm(asmtab * at, enum op op, unsigned int op0, unsigned int op1, unsigned int op2);
int reduce_cop(asmtab * at);
int jump_nop(asmtab * at, unsigned int ln);
int jump_cnd(asmtab * at);
void execute(asmtab * at, unsigned int* data, unsigned int max);
void export_asm(asmtab * at, FILE* out);

#endif
