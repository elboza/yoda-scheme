#ifndef ENV_H
#define ENV_H

object_t *make_symbol(char *value);
char is_symbol(object_t *obj);
object_t *make_fixnum(long value);
char is_fixnum(object_t *obj);
object_t *make_integer(long value);
char is_integer(object_t *obj);
object_t *make_float(float value);
char is_float(object_t *obj);
object_t *make_string(char *value);
char is_string(object_t *obj);
object_t *cons(object_t *car, object_t *cdr);
char is_pair(object_t *obj);
object_t *car(object_t *pair);
void set_car(object_t *obj, object_t *value);
object_t *cdr(object_t *pair);
void set_cdr(object_t *obj, object_t *value);
#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))




object_t *enclosing_environment(object_t *env);
object_t *first_frame(object_t *env);
object_t *make_frame(object_t *variables, object_t *values);
object_t *frame_variables(object_t *frame);
object_t *frame_values(object_t *frame);
void add_binding_to_frame(object_t *var, object_t *val, object_t *frame);
object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env);
object_t *lookup_variable_value(object_t *var, object_t *env);
void set_variable_value(objectt *var, object_t *val, object_t *env);
void define_variable(object_t *var, object_t *val, object_t *env);
object_t *setup_environment(void);
void populate_environment(object_t *env);
object_t *make_environment(void);
void init_env(void);

#endif
