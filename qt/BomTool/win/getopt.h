#ifndef WINGETOPT_H
#define WINGETOPT_H

#include <stdio.h>

extern char *optarg;
extern int   optind;
extern int   opterr;

extern int getopt(int argc, char *argv[], char *opstring);

#endif // WINGETOPT_H