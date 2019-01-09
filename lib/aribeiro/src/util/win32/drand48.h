#ifndef drand_48_h
#define drand_48_h
//
// cria defini��o do drand48, caso esteja no windows, � necess�rio compilar 
//    drand48.cpp e linkar ao projeto
//
#include <stdlib.h>

#if defined(_WIN32)

extern "C" {
  extern double	drand48(void);
  extern void	srand48(long int);
  extern unsigned short * seed48(unsigned short int [3]);
}

#endif

#endif
