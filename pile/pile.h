#ifndef PILE_H__
#define PILE_H__

union type {
   void* vide;
   int entier;
};

typedef struct {
   char* nom;
   unsigned short type;
   unsigned int profondeur;
   union type donnee;
} variable;

struct cellule;
typedef struct cellule {
   variable var;
   struct cellule* suivant;
} cellule;

typedef struct {
   unsigned int taille;
   cellule* debut;
} name;

#endif
