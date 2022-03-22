// test des opérations avec deux variables et un entier
main(){
    int multiVar ; 
    int var1 = 5 ; int var2 = 10 ;
    // sans parenthèses
    multiVar = var1 + var2 + 3; print(multiVar);
    multiVar = var1 + var2 - 3; print(multiVar);
    multiVar = var1 + var2 * 3; print(multiVar);
    multiVar = var1 + var2 / 3; print(multiVar);
    // avec parenthèses
    multiVar = (var1 + var2) + 3; print(multiVar);
    multiVar = (var1 + var2) - 3; print(multiVar);
    multiVar = (var1 + var2) * 3; print(multiVar);
    multiVar = (var1 + var2) / 3; print(multiVar);
}