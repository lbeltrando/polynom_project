#include <stdio.h>
#include <inttypes.h>

#include "f2_poly.h"

f2_deg_t f2_poly_deg(f2_poly_t p)
{
  int position = 0;
  int sizeMask = 32;
  if(((p & (f2_poly_t)1) == (f2_poly_t)1) && ((p & (~(f2_poly_t)1)) == (f2_poly_t)0)) // Si P = 1
    return 0;
  while(sizeMask >= 1) {
    if((p >> sizeMask) != 0) {
      p = p >> sizeMask;
      position += sizeMask;
    }
    else if((p & MASK(sizeMask)) != 0) {
      p = p & MASK(sizeMask);
    }
    else
      return 0;
    sizeMask = sizeMask/2;
  }
  return position;
  // Version naive :
  /*
    int i;
    f2_deg_t d = 63;
    if(p == (f2_poly_t)0 || p == (f2_poly_t)1)
      return (f2_poly_t)0;
    for(i = 63 ; i >= 0 ; i--) {
        if(((((f2_poly_t)1 << i) & p) >> i) == 0)
            d--;
        else
            break;
    }
    return d;
    */
}
// Probleme l'affichage des polynomes ne marche que pour des tailles < 32 !!!!!!!
// Resolu en utilisant le type uint64_t ou mieux f2_poly_t !! a changer dans les autres fonctions !!
int f2_poly_print(f2_poly_t p, char var, FILE * os)
{
    int i;
    f2_deg_t degP = f2_poly_deg(p);
    for(i = degP ; i > 1 ; i--) {
        if( ((( ((f2_poly_t)1) << i) & p) >> i) == 1) {
          fprintf(os, "%c^%d", var, i);
          if((MASK(i) & p) > 0)
            fprintf(os, "+");
        }
    }
    if(((((f2_poly_t)2 & p)) >> 1) == 1) {
      fprintf(os, "%c", var);
      if((((f2_poly_t)MASK(1)) & p) > 0 && (p >> 1) > 0)
        fprintf(os, "+1");
    }
    else if(((f2_poly_t)(MASK(1)) & p) > 0)
      fprintf(os,"1");
    if(p == 0)
      fprintf(os,"0");
    return 0;
}
// �criture polynomiale usuelle, avec pour variable le second argument

int f2_poly_div(f2_poly_t *q, f2_poly_t *r, f2_poly_t a, f2_poly_t b)
{
  *q = (f2_poly_t)0;
  *r = a;
  f2_deg_t degR = f2_poly_deg(a);
  f2_deg_t degB = f2_poly_deg(b);

  if(a == (f2_poly_t) 0) {
    *q = (f2_poly_t)0;
    *r = (f2_poly_t)0;
    return 0;
  }
  if(b == (f2_poly_t) 0) {
    return -1;
  }
  if(b == (f2_poly_t) 1) {
    *q = a;
    *r = (f2_poly_t)0;
    return 0;
  }
  if(a == b) {
    *q = (f2_poly_t)1;
    *r = (f2_poly_t)0;
  }
  while(degR >= degB) {
    *q = *q + ((f2_poly_t)1<<(degR - degB));
    *r = (*r)^(b << (degR - degB));
    degR = f2_poly_deg(*r);
  }
  return 0;
}
// (arg1, arg2) = (quotient, reste) de arg3 par arg4

f2_poly_t f2_poly_rem(f2_poly_t a, f2_poly_t b)
{
  f2_poly_t r = a;
  f2_deg_t degR = f2_poly_deg(r);
  f2_deg_t degB = f2_poly_deg(b);
  if(b == (f2_poly_t) 0) {
    printf("Erreur division par 0 :\n");
    return (f2_poly_t)0;
  }
  if(b == (f2_poly_t) 1 || a == b) {
    return (f2_poly_t)0;
  }
  if(degR == degB)
    return a^b;
  while(degR >= degB) {
    r = (r)^(b << (degR - degB));
    degR = f2_poly_deg(r);
  }
  return r;
}
// reste de arg1 par arg2

f2_poly_t f2_poly_gcd(f2_poly_t a, f2_poly_t b)
{
  if(f2_poly_deg(a)<f2_poly_deg(b))
    return f2_poly_gcd(b,a);
  f2_poly_t r = b;
  f2_poly_t pgcd = a;
  while(r!=0){
    pgcd = r;
    r = f2_poly_rem(a,b);
    a = b;
    b = r;
  }
  return pgcd;
}


/* Pour tous les calculs modulo. On peut  quotienter par un
polyn�me de degr� jusqu'� 63. */

f2_poly_t f2_poly_xtimes(f2_poly_t p, f2_poly_t n)
{
  p = f2_poly_rem(p,n); // On reduit p mod n
  p = p << 1; // multiplication par X
  if(f2_poly_deg(p) == f2_poly_deg(n)) {
    p = p^n; // xor pour faire p = p - q
  }
  return p;
}
// retourne X*arg1 mod (arg2)


f2_poly_t f2_poly_times(f2_poly_t a, f2_poly_t b, f2_poly_t n)
{
  f2_poly_t res = 0;
  f2_deg_t degA = f2_poly_deg(a);
  int i;
  for(i = 0 ; i <= degA ; i++) {
    if(((((f2_poly_t)1 << i) & a) >> i) == 1) {
      res = res^b; // xor pour faire r + b
      res = f2_poly_rem(res,n);
    }
    b = f2_poly_xtimes(b,n);
  }
  return res;
}
  // retourne arg1 * arg2 modulo  arg3

f2_poly_t f2_poly_x2n(f2_deg_t a, f2_poly_t n)
{
  if(a == (f2_poly_t)0)
    return (f2_poly_t)1;
  f2_poly_t res = 0x4; // X^2 = 100 en binaire = 0x4
  int i;
  // On fait (a-1) mise au carre avec modulo successives
  for(i = 1 ; i < a ; i++) {
    res = f2_poly_times(res,res,n);
  }
  return res;
}
// retourne X^{2^arg1} modulo arg 2

f2_poly_t f2_poly_parity(f2_poly_t p)
{
  // On va utiliser le MASK on va faire un xor entre une moitie et l'autre
  f2_poly_t res = p;
  uint8_t sizeMask = 32;
  while(sizeMask >= 1) {
    res = (res & (f2_poly_t)MASK(sizeMask))^((res & (f2_poly_t)MASK(2*sizeMask)) >> sizeMask);
    sizeMask = sizeMask >> 1;
  }
  return res;
}
//retourne le reste de la division par X+1 (xor des bits)

f2_poly_t f2_poly_recip(f2_poly_t p)
{
  f2_poly_t res = 0;
  f2_deg_t degP = f2_poly_deg(p);
  int i;
  int j = 0;

  for(i = degP ; i >= 0 ; i--) {
    if(((((f2_poly_t)1 << i) & p) >> i) == 1)
      res = res + ((f2_poly_t)1 << j);
    j++;
  }
  return res;
}
// retourne le polyn�me r�ciproque consid�r� comme de degr� le second argument

/* pour renverser l'ordre des bits sur 64 bits :
     polP = ((polP & 0x5555555555555555ULL) << 1)
	  ^ ((polP & 0xaaaaaaaaaaaaaaaaULL) >> 1);
     polP = ((polP & 0x3333333333333333ULL) << 2)
	  ^ ((polP & 0xccccccccccccccccULL) >> 2);
     polP = ((polP & 0x0f0f0f0f0f0f0f0fULL) << 4)
	  ^ ((polP & 0xf0f0f0f0f0f0f0f0ULL) >> 4);
     polP = ((polP & 0x00ff00ff00ff00ffULL) << 8)
	  ^ ((polP & 0xff00ff00ff00ff00ULL) >> 8);
     polP = ((polP & 0x0000ffff0000ffffULL) << 16)
	  ^ ((polP & 0xffff0000ffff0000ULL) >> 16);
     polP = ((polP & 0x00000000ffffffffULL) << 32)
	  ^ ((polP & 0xffffffff00000000ULL) >> 32);
*/

int f2_poly_irred(f2_poly_t p) // TO DO
{ // retourne 0 si p est irreductible, -1 sinon
  /* On va utiliser le critere suivant d'irreductibilite des polynomes a coeff dans Fq :
    Un polynome P de degre n > 0 sur Fq est irreductible ssi :
      - P | X^{q^n} - X
      - P est premier avec les X^{q^r} - X ou r = n/d, d diviseur premier de n
  */
  if(p == (f2_poly_t)2)
    return 0;
  if((f2_poly_parity(p) == (f2_poly_t)0) && (p > (f2_poly_t)3)) // X+1 irreductible
    return -1;
  if((p & (f2_poly_t)1) == (f2_poly_t)0) // c'est que le poly est divisible par x
    return -1;
  f2_deg_t degP = f2_poly_deg(p);
  f2_deg_t tmpDeg = 0;
  f2_poly_t tmp = f2_poly_x2n(degP,p); // tmp = X^{2^n}
  tmp = tmp^((f2_poly_t)2); // tmp = X^{2^n} - X
  if(f2_poly_rem(tmp,p) != 0) {
    return -1;
  }
  // Si on arrive ici c'est que P | X^{q^n} - X
  // Il faut dresser la liste des diviseur premier de degP
  while(degP > 1) {

    tmpDeg = pp_diviseur_premier(degP);
    tmp = f2_poly_x2n(f2_poly_deg(p)/tmpDeg,p); // tmp = X^{2^r}, ou r = n/d, d diviseur premier de n
    tmp = tmp^((f2_poly_t)2);
    if(f2_poly_gcd(p,tmp) != (f2_poly_t)1) { // Si P n'est pas premier avec X^{2^r}
      return -1;
    }
    degP = degP / tmpDeg;
    while(pp_diviseur_premier(degP) == tmpDeg) {
      degP = degP / tmpDeg;
    }
  }
  return 0;
}
// v�rifie si le polyn�me arg1 est irr�ductible


f2_poly_t f2_poly_xn(f2_poly_t a, f2_poly_t n)
{ // x^a mod n
  if(a == (f2_poly_t)0)
    return (f2_poly_t)1;
  f2_poly_t res = 1;
  f2_poly_t x = 0x2; // X = 10 en binaire = 0x2
  int i;
  for(i = 0 ; i <= f2_poly_deg(a) ; i++) {
    if(((((f2_poly_t)1 << i) & a) >> i) == (f2_poly_t)1)
      res = f2_poly_times(res,x,n);
    x = f2_poly_times(x,x,n);
  }
  return res;
}
// retourne X^{arg1} modulo arg 2


int f2_poly_primitive(f2_poly_t p) {
  if(f2_poly_irred(p) == -1)
    return -1;
  f2_deg_t degP = f2_poly_deg(p);
  f2_poly_t tmp = MASK(degP); // tmp = 2^n - 1
  f2_poly_t tmpBis = tmp;
  f2_poly_t di = pp_diviseur_premier(tmp);
  while(tmpBis > (f2_poly_t)1) {
    // Si P ne divise pas X^{(2^n - 1)/ r } - 1 | r diviseur premier de 2^n - 1 :
    if(f2_poly_xn((tmp/di),p) == (f2_poly_t)1) {
      return -1;
    }
    while(pp_diviseur_premier(tmpBis) == di) {
      tmpBis = tmpBis / di;
    }
    di = pp_diviseur_premier(tmpBis);
  }
  return 0;
}
// v�rifie si le polyn�me arg1 est primitif
uint64_t f2_poly_irred_order(f2_poly_t polP)
{
  if(f2_poly_irred(polP) == -1)
    return (uint64_t)0;
  f2_poly_t n = (f2_poly_t)1;
  f2_poly_t racine = (f2_poly_t)2;
  while(racine != (f2_poly_t)1) {
    racine = f2_poly_xtimes(racine,polP);
    n = n + (f2_poly_t)1;
  }
  return n;
}
// renvoie 0 si le polyn�me polP n'est pas irr�ductible,
// l'ordre multiplicatif d'une racine, soit de  X modulo P sinon
// Si cet ordre �gale 1 << f2_poly-deg(P) - 1, le polyn�me est primitif.

f2_poly_t f2_poly_random_inf(f2_deg_t n)
{
  FILE *urandom;
  unsigned int alea;
  double random = 0;
  urandom = fopen(RANDOMFILE,"r");
  if(urandom == NULL) {
    fprintf(stderr,"%s cannot open",RANDOMFILE);
    fclose(urandom);
    return -1;
  }
  fread(&alea,sizeof(alea),1,urandom);
  fclose(urandom);

  int i;
  f2_poly_t res = 0;
  srand(alea); // alea nous fournit un nombre aleatoire qui va servir a generer une suite de nombre pseudo-aleatoire grace a rand();
  for(i = n ; i >= 0 ; i--) {
    random = rand()/(double)RAND_MAX;//RAND_MAX;
    if(random > 0.5)
      res = res + ((f2_poly_t)1<<i);
  }
  return res;
}
// retourne un polyn�me tir� au hasard parmi les polyn�mes de degr� < arg

f2_poly_t f2_poly_random(f2_deg_t n)
{
  f2_poly_t res = f2_poly_random_inf(n);
  if((res >> n) == 0)
    res = res + ((f2_poly_t)1 << n);
  return res;
}
// retourne un polyn�me tir� au hasard parmi les polyn�mes de degr� = arg

f2_poly_t f2_poly_irred_random(f2_deg_t n)
{
  f2_poly_t p = f2_poly_random(n);
  while(f2_poly_irred(p) == -1)
    p = f2_poly_random(n);
  return p;
}
  // retourne un polyn�me tir� au hasard parmi les polyn�mes irr�ductibles
  // de degr� = arg2

f2_poly_t f2_poly_primitive_random(f2_deg_t n)
{
  f2_poly_t p = f2_poly_random(n);
  while(f2_poly_primitive(p) == -1)
    p = f2_poly_random(n);
  return p;
}
// retourne un polyn�me tir� au hasard parmi les polyn�mes primitifs
// de degr� = arg2

int f2_poly_irred_count(f2_deg_t n)
{
  int res = 0;
  f2_poly_t start = (f2_poly_t)1 << n;
  f2_poly_t end = (f2_poly_t)MASK((n + (f2_deg_t)1));
  while(start <= end) {
    if(f2_poly_irred(start) == 0)
      res++;
    start = start + (f2_poly_t)1;
  }
  return res;
}

int f2_poly_primitive_count(f2_deg_t n)
{
  int res = 0;
  f2_poly_t start = (f2_poly_t)1 << n;
  f2_poly_t end = (f2_poly_t)MASK((n + (f2_deg_t)1));
  while(start <= end) {
    if(f2_poly_primitive(start) == 0)
      res++;
    start = start + (f2_poly_t)1;
  }
  return res;
}
