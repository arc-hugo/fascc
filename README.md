# FASCC
Première partie du Projet Systèmes Informatiques visant à réaliser un compilateur de C simplifié grâce à LEX/YACC.  
FASCC permet de compiler un code C minimal en code assembleur orienté mémoire.  
Le code assembleur en sortie peut ensuite être interprété grâce à l'utilitaire FASTINTERP.

## Fonctionnalités
- [x] Implémentation des expressions séquentielles classiques (déclaration, affectation, affichage, opérations arithmétiques).
- [x] Implémentation des constantes à la compilation.
- [x] Création d'un interpréteur du code ASM.
- [x] Implémentation des expressions conditionnelles (if et while).
- [ ] Implémentation de pointeurs
- [x] Implémentation de fonctions
- [ ] Traitement des erreurs
- [ ] Optimisation du code ASM

## Prérequis

- [bash](https://www.gnu.org/software/bash/)
- [gcc](https://gcc.gnu.org/)
- [make](https://www.gnu.org/software/make/)

## Utilisation

Les commandes doivent être lancées dans le répertoire courant.

Les executables FASCC et FASTINTERP sont obtenus grâce à la commande :
```bash
$ make
```
La compilation d'un fichier C s'effectue ensuite sous la forme :
```bash
$ ./fascc chemin/vers/fichier.c
```
Le fichier de sortie se nom ```out``` par défaut et est placé dans le dossier courant.  
L'option ```-o chemin/vers/sortie``` peut être utilisée pour appliquer un nouveau nom.

Les fichiers assembleur en sortie peuvent ensuite être interprété avec :
```bash
$ ./fastinterp chemin/vers/sortie
```

Plusieurs exemples de code compilables sont disponibles dans le dossier [test](test/).  
Ces fichhiers servent également de test pour le compilateur et peuvent être lancés via :
```bash
$ make test
```
Pour lancer une selection de fichiers test, la commande suivante peut-être utilisée :
```bash
$ make test TESTFILES="chemin/vers/test1.c chemin/vers/test2.c ... chemin/vers/testN.c"
```
