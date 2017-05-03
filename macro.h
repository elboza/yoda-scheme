#ifndef MACRO_H
#define MACRO_H

object_t* is_define_macro(object_t *obj);
int is_macro(object_t *obj);
object_t* define_macro(object_t *obj);
object_t* expand_macro(object_t *obj);
object_t* macro(object_t *obj);

#endif