#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include "arithm.h"


//const int prime[PRIME64_SIZE] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61};
// Retourne le plus petit diviseur premier

uint64_t pp_diviseur_premier(uint64_t n)
{
  if((n == (uint64_t)1) || (n == (uint64_t)0))
    return n;
  if((n & (uint64_t)1) == (uint64_t)0)
    return (uint64_t)2;
  uint64_t i = (uint64_t) 3;
  uint64_t sqrn = ceil(sqrt(n));
  while(i <= sqrn) {
  if(n % i == (uint64_t)0)
    return i;
  i = i + (uint64_t)2;
  }
  return n;
}
