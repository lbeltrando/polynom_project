#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "f2_poly.h"

int main(int argc, char *argv[])
{
    f2_deg_t x,y;
    if(argc > 3) {
      printf("Entrez le bon nombre d'argument !\n");
      return 0;
    }
    if(argc == 1) {
      x = 1;
      y = 15;
    }
    if(argc == 2) {
      x = atoi(argv[1]);
      y = x;
    }
    if(argc == 3) {
      x = atoi(argv[1]);
      y = atoi(argv[2]);
    }
    if(x > 0 && x <= y && y <= 63) {
      printf("\n\n");
      while(x <= y) {
        printf("Nombre de polynomes primitifs de degre %d = %d\n",x,f2_poly_primitive_count(x));
        x++;
      }
      printf("\n\n");
    }
    return 0;
}
