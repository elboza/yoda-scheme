/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef MAIN_H
#define MAIN_H

#define DEBUG 1
//#define DEBUG 0
#define HAVE_LIBREADLINE
//#undef HAVE_LIBREADLINE
#define FILENAME_LEN 255
#define VERSION "0.1"
#define PROGNAME "Yoda-Scheme"
#define TARGETNAME "yscm"

void log_d(char *s);
void die(char *s);


#endif
