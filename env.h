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
object_t *make_character(char value);
char is_character(object_t *obj);
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

object_t *make_primitive_proc(object_t *(*fn)(struct object_t *arguments));
char is_primitive_proc(object_t *obj);
object_t *is_null_proc(object_t *arguments);
object_t *is_boolean_proc(object_t *arguments);
object_t *is_symbol_proc(object_t *arguments);
object_t *is_integer_proc(object_t *arguments);
object_t *is_float_proc(object_t *arguments);
object_t *is_number_proc(object_t *arguments);
object_t *is_char_proc(object_t *arguments);
object_t *is_string_proc(object_t *arguments);
object_t *is_pair_proc(object_t *arguments);
object_t *is_procedure_proc(object_t *arguments);
object_t *char_to_integer_proc(object_t *arguments);
object_t *char_to_float_proc(object_t *arguments);
object_t *char_to_number_proc(object_t *arguments);
object_t *integer_to_char_proc(object_t *arguments);
object_t *float_to_char_proc(object_t *arguments);
object_t *number_to_char_proc(object_t *arguments);
object_t *number_to_string_proc(object_t *arguments);
object_t *string_to_number_proc(object_t *arguments);
object_t *symbol_to_string_proc(object_t *arguments);
object_t *string_to_symbol_proc(object_t *arguments);
char are_number_args(object_t *arguments);
char are_float_args(object_t *arguments);
object_t *add_proc(object_t *arguments);
object_t *sub_proc(object_t *arguments);
object_t *mul_proc(object_t *arguments);
object_t *quotient_proc(object_t *arguments);
object_t *remainder_proc(object_t *arguments);
object_t *is_number_equal_proc(object_t *arguments);
object_t *is_less_than_proc(object_t *arguments);
object_t *is_greater_than_proc(object_t *arguments);
object_t *cons_proc(object_t *arguments);
object_t *car_proc(object_t *arguments);
object_t *cdr_proc(object_t *arguments);
object_t *set_car_proc(object_t *arguments);
object_t *set_cdr_proc(object_t *arguments);
object_t *list_proc(object_t *arguments);
object_t *is_eq_proc(object_t *arguments);
object_t *apply_proc(object_t *arguments);
object_t *interaction_environment_proc(object_t *arguments);
object_t *null_environment_proc(object_t *arguments);
object_t *environment_proc(object_t *arguments);
object_t *eval_proc(object_t *arguments);
object_t *append_proc(object_t *arguments);
object_t *load_proc(object_t *arguments);
object_t *open_input_port_proc(object_t *arguments);
object_t *close_input_port_proc(object_t *arguments);
object_t *is_input_port_proc(object_t *arguments);
object_t *read_proc(object_t *arguments);
object_t *read_char_proc(object_t *arguments);
object_t *peek_char_proc(object_t *arguments);
object_t *is_eof_object_proc(object_t *arguments);
object_t *open_output_port_proc(object_t *arguments);
object_t *close_output_port_proc(object_t *arguments);
object_t *is_output_port_proc(object_t *arguments);
object_t *write_char_proc(object_t *arguments);
object_t *write_proc(object_t *arguments);
object_t *display_proc(object_t *arguments);
object_t *debug_proc(object_t *arguments);
object_t *error_proc(object_t *arguments);
object_t *make_compound_proc(object_t *parameters, object_t *body, object_t *env);
char is_compound_proc(object_t *obj);
object_t *make_input_port(FILE *stream);
char is_input_port(object_t *obj);
object_t *make_output_port(FILE *stream);
char is_output_port(object_t *obj);
char is_eof_object(object_t *obj);


object_t *enclosing_environment(object_t *env);
object_t *first_frame(object_t *env);
object_t *make_frame(object_t *variables, object_t *values);
object_t *frame_variables(object_t *frame);
object_t *frame_values(object_t *frame);
void add_binding_to_frame(object_t *var, object_t *val, object_t *frame);
object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env);
object_t *lookup_variable_value(object_t *var, object_t *env);
void set_variable_value(object_t *var, object_t *val, object_t *env);
void define_variable(object_t *var, object_t *val, object_t *env);
object_t *setup_environment(void);
void populate_environment(object_t *env);
object_t *make_environment(void);
void init_env(void);

#endif
