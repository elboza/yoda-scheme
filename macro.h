#ifndef MACRO_H
#define MACRO_H

int is_define_macro(object_t *obj);
object_t* is_macro(object_t *obj);
object_t* define_macro(object_t *obj);
object_t* expand_macro(object_t *obj,object_t *vals);
object_t* macro(object_t *obj);
object_t *lookup_macro(object_t *var, object_t *env);

#endif