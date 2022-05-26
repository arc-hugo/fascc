void f(int a) {
   print(a);
   if (a < 10) {
      f(a+1);
   }
}

int main() {
   f(0+3);
}
