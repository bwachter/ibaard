#ifndef _AM_GETOPT_H
#define _AM_GETOPT_H

int	getopt(int argc, char * const argv[], const char *optstring);
char *optarg;
int optind, opterr, optopt;
#endif
