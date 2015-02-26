#ifndef OBJECT_H
#define OBJECT_H

typedef enum {T_NONE, T_BOTTOM, T_EMPTY_LIST, T_ATOM, T_SYMBOL, T_STRING, T_NUMBER, T_INTEGER, T_FLOAT, T_PAIR, T_PRIMITIVE_PROC, T_COMPOUND_PROC, T_LAMBDA, T_DEFINE, T_CHARACTER, T_BOOLEAN, T_INPUT_PORT, T_OUTPUT_PORT, T_EOF_OBJECT, T_WARN_MSG } object_type;

typedef struct object_t {
	object_type type;
	union{
		struct {
			char value;
		}boolean;
		struct{
			long value;
		}fixnum;
		struct{
			float value;
		}dotted;
		struct {
			char *value;
		} string;
		struct {
			char *value;
		}symbol;
		struct {
			char value;
		}character;
		struct {
			struct object_t *(*fn)(struct object_t *arguments);
		}primitive_proc;
		struct {
			struct object_t *parameters;
			struct object_t *body;
			struct object_t *env;
		}compound_proc;
		struct {
			struct object_t *car;
			struct object_t *cdr;
		}pair;
		struct {
			FILE *stream;
		}input_port;
		struct {
			FILE *stream;
		}output_port;
	}data;
} object_t;

object_t* alloc_object(void);

object_t *the_empty_list;
object_t *false;
object_t *true;
object_t *bottom;
object_t *symbol_table;
object_t *quote_symbol;
object_t *define_symbol;
object_t *set_symbol;
object_t *ok_symbol;
object_t *if_symbol;
object_t *lambda_symbol;
object_t *begin_symbol;
object_t *cond_symbol;
object_t *else_symbol;
object_t *let_symbol;
object_t *and_symbol;
object_t *or_symbol;
object_t *quasiquote_symbol;
object_t *unquote_symbol;
object_t *unquote_splicing_symbol;
object_t *letstar_symbol;
object_t *eof_object;
object_t *the_empty_environment;
object_t *the_global_environment;

object_t *cons(object_t *car, object_t *cdr);
object_t *car(object_t *pair);
object_t *cdr(object_t *pair);

char is_the_empty_list(object_t *obj);
char is_boolean(object_t *obj);
char is_false(object_t *obj);
char is_true(object_t *obj);
char is_bottom(object_t *obj);
char is_number(object_t *obj);


#endif
