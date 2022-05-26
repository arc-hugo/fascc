void f2(int b) {
   print(b);
}

void f(int a, int b) {
   print(a);
   f2(b);
}

int main() {
   int a = 20;
   int b = 30;
   f(a,b);
}
