#include <iostream>
#include <math.h>

#define ITERATIONS 100

float fac(int n) {
  if (n==0)
    return 1;
  else
    return n*fac(n-1);
}

// Doesn't work well
// int pow(int base, int ex) {
//   if (ex == 0)
//     return 1;
//   else if (ex % 2 == 0)
//     return pow(base, ex / 2) * pow(base, ex / 2);
//   else
//     return base * pow(base, ex / 2) * pow(base, ex / 2);
// }

float sen(float x) {
  float sum = 0;
  int k = 0;

  int c;
  for (int k = 0; k <= ITERATIONS; k++) {
    c = (k%2) == 0 ? 1 : -1;
    sum += (c*pow(x, 2*k+1))/(fac(2*k+1));
    std::cout << k << ": " << sum << "\n";
  }

  return sum;
}

int main() {
  float input;

  std::cout << "Digite o número cujo sen sera aproximado: ";
  std::cin >> input;
  std::cout << "\n";

  float n = sen(input);

  std::cout << "\n";
  std::cout << "O sen é " << n;
}