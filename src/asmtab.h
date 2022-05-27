#ifndef ASMTAB_H__
#define ASMTAB_H__

enum op {
   NOP = 0, // Instruction vide
   ADD = 1, // Addition des valeurs de deux adresses vers une autre
   SOU = 2, // Soustraction des valeurs de deux adresses vers une autre
   MUL = 3, // Multiplication des valeurs de deux adresses vers une autre
   DIV = 4, // Division deq valeurs de deux adresses vers une autre
   AND = 5, // ET logique entre deux valeurs vers une adresse
   OR = 6, // OU logique entre deux valeurs vers une adresse
   NOT = 7, // NON logique d'une valeurs vers une adresse
   COP = 8, // Copie la valeur d'une adresse vers une autre adresse
   AFC = 9, // Affecte une valeur à une adresse
   JMP = 10, // Saut inconditionnel vers une instruction
   JMF = 11, // Saut vers une instruction si condition dans adresse est fausse
   INF = 12, // Comparaison inférieur des valeurs de deux adresses vers une autre
   SUP = 13, // Comparaison supérieur des valeurs de deux adresses vers une autre
   EQU = 14, // Comparaison égale de deux valeurs de deux adresses vers une adresse
   PRI = 15, // Affichage de la valeur d'une adresse
   CLL = 16, // Sauvegarde de l'adresse de retour et décalage du base pointer lors d'un appel de fonction
   RET = 17, // Décalage du base pointer et sortie vers l'adresse de retour de la fonction 
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

int jump_if(asmtab * at, unsigned int pos, unsigned int ln);
int jump_while(asmtab * at, unsigned int pos, unsigned int ret, unsigned int ln);
void set_main_asm(asmtab * at, unsigned int ln);

#endif
