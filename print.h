#ifndef PRINT_H
#define PRINT_H

void write_pair(FILE *out, object_t *pair);
void write_sx(FILE *out, object_t *obj);
void write_debug(FILE *out, object_t *obj);
void display_pair(FILE *out, object_t *pair);
void display_sx(FILE *out, object_t *obj);

#endif
