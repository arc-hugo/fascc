main(){
   int a = 0;
   while(a < 10){
      while(a > 5 && a < 20){
         print(a);
         a = a + 1;
      }
      a = a + 1;
   }
   print(a);
}
