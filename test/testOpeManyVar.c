/* test des opérations avec plus de deux variables */
main(){
    int multiVar2 ;
    int var1 = 5 ; int var2 = 10 ; int var3 = 5 ; int var4 = 10;
    multiVar2 = var1 + var2 + var3 - var4 ; print(multiVar2); // + et - 
    multiVar2 = var1 + var2 + var3 * var4 ; print(multiVar2); // * à la fin 
    multiVar2 = var1 + var2 + var3 / var4 ; print(multiVar2); // / à la fin
    multiVar2 = var1 / var2 + var3 * var4 - var3 ; print(multiVar2); // tout dans le désordre 
}