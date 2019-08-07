#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "f2_poly.h"

int main(int argc, char *argv[])
{
  f2_deg_t n;
  if(argc != 2) {
      printf("Entrez en argument le degre des polynomes a generer !\n");
      return 0;
  }
  else {
      n = atoi(argv[1]);
  }
  if(n <= 0 || n >= 64) {
    printf("Entrez un argument compris (strictement) entre 1 et 63\n");
    return 0;
  }
  printf("\n\nGeneration d'un polynome aleatoire quelconque de degre inferieur (ou egal) a %d :\n  ",n);
  f2_poly_print(f2_poly_random_inf(n),'X',stdout);
  printf("\n\nGeneration d'un polynome aleatoire quelconque de degre %d :\n  ",n);
  f2_poly_print(f2_poly_random(n),'X',stdout);
  printf("\n\nGeneration d'un polynome aleatoire irreductible de degre %d :\n  ",n);
  f2_poly_print(f2_poly_irred_random(n),'X',stdout);
  printf("\n\nGeneration d'un polynome aleatoire primitif de degre %d :\n  ",n);
  f2_poly_print(f2_poly_primitive_random(n),'X',stdout);
  printf("\n\n");

  return 0;
}
