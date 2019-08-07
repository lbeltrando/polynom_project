#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "f2_poly.h"
void separation();
void afficheDegre(f2_poly_t p, char c);
void afficheDiv(f2_poly_t a, f2_poly_t b);
void afficheRem(f2_poly_t a,f2_poly_t b);
void affichePgcd(f2_poly_t a,f2_poly_t b);
void afficheXtimes(f2_poly_t p, f2_poly_t n);
void affichePolytimes(f2_poly_t a,f2_poly_t b, f2_poly_t n);
void afficheX2n(f2_deg_t a, f2_poly_t n);
void afficheParity(f2_poly_t p);
void afficheRecip(f2_poly_t p);
void afficheXn(f2_poly_t a,f2_poly_t n);
void afficheIrredPrim(f2_poly_t p);
void afficheIrredOrder(f2_poly_t p);

int main()
{
  printf("\nVoici un programme pour tester les fonctions de la bibliothèque :");
  separation();
  printf("Test des fonctions f2_poly_deg et f2_poly_print :\n\n");
  f2_poly_t polA = 0x3f,polB = 0x19,polC = 0x7,polD = 0x22, polE = 0xffeeaa17, polF = 0x1287feab18ba18;
  f2_poly_t polAES = 0x11b;     //X^8+X^4+X^3+X+1, irréductible non primitif
  f2_poly_t polA51a = 0xe4001;  //X^19+X^18+X^17+X^14+1, irréductible primitif
  f2_poly_t polA51b = 0x600001; //X^22+X^21+1, irréductible primitif
  f2_poly_t polA51c = 0xe00101; //X^23+X^22+X^21+X^8+1, irréductible primitif
  afficheDegre(polA,'X');
  afficheDegre(polB,'Y');
  afficheDegre(polC,'Z');
  afficheDegre(polE,'X');
  afficheDegre(polF,'X');
  separation();
  printf("Test de la fonction f2_poly_div :\n\n");
  afficheDiv(polA,polB);
  printf("\n\n");
  afficheDiv(polB,polC);
  separation();
  printf("Test de la fonction f2_poly_rem :\n\n");
  afficheRem(polA,polB);
  separation();
  printf("Test de la fonction f2_gcd :\n\n");
  affichePgcd(polA,polB);
  affichePgcd(polB,polC);
  affichePgcd(polA,polD);
  separation();
  printf("Test de la fonction f2_poly_xtimes :\n\n");
  afficheXtimes(polC,polA);
  afficheXtimes(polB,polA);
  afficheXtimes(polB,polC);
  separation();
  printf("Test de la fonction f2_poly_times :\n\n");
  affichePolytimes(polB,polC,polA);
  separation();
  printf("Test de la fonction f2_poly_x2n :\n\n");
  afficheX2n(3,polA);
  afficheX2n(5,polC);
  separation();
  printf("Test de la fonction f2_poly_parity :\n\n");
  afficheParity(polA);
  afficheParity(polC);
  afficheParity(polE);
  separation();
  printf("Test de la fonction f2_poly_recip :\n\n");
  afficheRecip(polA);
  afficheRecip(polD);
  afficheRecip(polB);
  separation();
  printf("Test de la fonction f2_poly_xn :\n\n");
  afficheXn(polA,polB);
  afficheXn(polE,polA);
  separation();
  printf("Test des fonctions f2_poly_irred et f2_poly_primitive :\n\n");
  afficheIrredPrim(polAES);
  afficheIrredPrim(polA51a);
  afficheIrredPrim(polA51b);
  afficheIrredPrim(polA51c);
  separation();
  printf("Test de la fonction f2_poly_irred_order :\nSi le polynome de degre n ");
  printf("est irreductible on calcule 2^n - 1 si c'est egale a l'ordre multiplicatif");
  printf(" de la racine alors le polynome est primitif:\n\n");
  afficheIrredOrder(polA);
  afficheIrredOrder(polB);
  afficheIrredOrder(polAES);
  afficheIrredOrder(polA51a);
  afficheIrredOrder(polA51b);
  afficheIrredOrder(polA51c);
  separation();
  printf("Generation d'un polynome primitif aleatoire de degre 23 et calcul de l'ordre multiplicatif d'une racine :\n\n");
  afficheIrredOrder(f2_poly_primitive_random(23));

  return 0;
}
void separation()
{
  printf("\n\n-------------------------------------------------------------\n\n");
}
void afficheDegre(f2_poly_t p,char c)
{
  printf("Le polynome ");
  f2_poly_print(p,c,stdout);
  printf(" est de degre %d\n", f2_poly_deg(p));
}
void afficheDiv(f2_poly_t a, f2_poly_t b)
{
  f2_poly_t q,r;
  f2_poly_div(&q,&r,a,b);
  printf("Division euclidienne de ");
  f2_poly_print(a,'X',stdout);
  printf(" par ");
  f2_poly_print(b,'X',stdout);
  printf(" :\nQuotient = ");
  f2_poly_print(q,'X',stdout);
  printf("\nReste = ");
  f2_poly_print(r,'X',stdout);
}
void afficheRem(f2_poly_t a, f2_poly_t b)
{
  printf("Reste de la division euclidienne de ");
  f2_poly_print(a,'X',stdout);
  printf(" par ");
  f2_poly_print(b,'X',stdout);
  printf(" = ");
  f2_poly_print(f2_poly_rem(a,b),'X',stdout);
}
void affichePgcd(f2_poly_t a, f2_poly_t b)
{
  printf("PGCD(");
  f2_poly_print(a,'X',stdout);
  printf(",");
  f2_poly_print(b,'X',stdout);
  printf(") = ");
  f2_poly_print(f2_poly_gcd(a,b),'X',stdout);
  printf("\n\n");
}
void afficheXtimes(f2_poly_t p, f2_poly_t n)
{
  f2_poly_print(p,'X',stdout);
  printf(" * X mod ");
  f2_poly_print(n,'X',stdout);
  printf(" = ");
  f2_poly_print(f2_poly_xtimes(p,n),'X',stdout);
  printf("\n\n");
}
void affichePolytimes(f2_poly_t a,f2_poly_t b, f2_poly_t n)
{
  f2_poly_print(a,'X',stdout);
  printf(" * ");
  f2_poly_print(b,'X',stdout);
  printf(" mod ");
  f2_poly_print(n,'X',stdout);
  printf(" = ");
  f2_poly_print(f2_poly_times(a,b,n),'X',stdout);
  printf("\n\n");
}
void afficheX2n(f2_deg_t a, f2_poly_t n)
{
  printf("X^(2^%d) mod ",a);
  f2_poly_print(n,'X',stdout);
  printf(" = ");
  f2_poly_print(f2_poly_x2n(a,n),'X',stdout);
  printf("\n\n");
}
void afficheParity(f2_poly_t p)
{
  printf("Reste de la division de ");
  f2_poly_print(p,'X',stdout);
  printf(" par X+1 = ");
  f2_poly_print(f2_poly_parity(p),'X',stdout);
  printf("\n\n");
}
void afficheRecip(f2_poly_t p)
{
  printf("Le polynome reciproque de ");
  f2_poly_print(p,'X',stdout);
  printf(" est ");
  f2_poly_print(f2_poly_recip(p),'X',stdout);
  printf("\n\n");
}
void afficheXn(f2_poly_t a,f2_poly_t n)
{
  printf("X^(%" PRIu64 ") mod ",a);
  f2_poly_print(n,'X',stdout);
  printf(" = ");
  f2_poly_print(f2_poly_xn(a,n),'X',stdout);
  printf("\n\n");
}
void afficheIrredPrim(f2_poly_t p)
{
  printf("Le polynome ");
  f2_poly_print(p,'X',stdout);
  printf("\nEst-il irreductible ? ");
  if(f2_poly_irred(p) == -1)
    printf("NON");
  else
    printf("OUI");
  printf("\nPrimitif ? ");
  if(f2_poly_primitive(p) == -1)
      printf("NON");
  else
    printf("OUI");
  printf("\n\n");
}
void afficheIrredOrder(f2_poly_t p)
{
  if(f2_poly_irred_order(p) == (uint64_t)0) {
    printf("Le polynome ");
    f2_poly_print(p,'X',stdout);
    printf(" n'est pas irreductible. ");
  }
  else {
    printf("X mod ");
    f2_poly_print(p,'X',stdout);
    printf(" a pour ordre multiplicatif %" PRIu64 "",f2_poly_irred_order(p));
    printf(" (2^%d - 1 = %" PRIu64 ")",f2_poly_deg(p),(uint64_t)(pow(2,f2_poly_deg(p)) - 1));
  }
  printf("\n\n");
}
