#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"
#include "eval.h"
#include "print.h"


object_t *make_symbol(char *value) {
	object_t *obj;
	object_t *element;
	
	/* search for they symbol in the symbol table */
	element = symbol_table;
	while (!is_the_empty_list(element)) {
		if (strcmp(car(element)->data.symbol.value, value) == 0) {
			return car(element);
		}
		element = cdr(element);
	};
	
	/* create the symbol and add it to the symbol table */
	obj = alloc_object();
	obj->type = T_SYMBOL;
	obj->data.symbol.value = malloc(strlen(value) + 1);
	if (obj->data.symbol.value == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	strcpy(obj->data.symbol.value, value);
	symbol_table = cons(obj, symbol_table);
	return obj;
}
char is_symbol(object_t *obj) {
	return obj->type == T_SYMBOL;
}
object_t *make_fixnum(long value) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_INTEGER;
	obj->data.fixnum.value = value;
	return obj;
}

char is_fixnum(object_t *obj) {
	return obj->type==T_INTEGER;
}
object_t *make_integer(long value) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_INTEGER;
	obj->data.fixnum.value = value;
	return obj;
}

char is_integer(object_t *obj) {
	return obj->type==T_INTEGER;
}
object_t *make_float(float value) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_FLOAT;
	obj->data.dotted.value = value;
	return obj;
}

char is_float(object_t *obj) {
	return(obj->type==T_FLOAT);
}
object_t *make_character(char value) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_CHARACTER;
	obj->data.character.value = value;
	return obj;
}

char is_character(object_t *obj) {
	return obj->type == T_CHARACTER;
}
object_t *make_string(char *value) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_STRING;
	obj->data.string.value = malloc(strlen(value) + 1);
	if (obj->data.string.value == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	strcpy(obj->data.string.value, value);
	return obj;
}

char is_string(object_t *obj) {
	return obj->type == T_STRING;
}
object_t *cons(object_t *car, object_t *cdr) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_PAIR;
	obj->data.pair.car = car;
	obj->data.pair.cdr = cdr;
	return obj;
}

char is_pair(object_t *obj) {
	return obj->type == T_PAIR;
}
object_t *car(object_t *pair) {
	return pair->data.pair.car;
}

void set_car(object_t *obj, object_t *value) {
	obj->data.pair.car = value;
}

object_t *cdr(object_t *pair) {
	return pair->data.pair.cdr;
}

void set_cdr(object_t *obj, object_t *value) {
	obj->data.pair.cdr = value;
}

object_t *make_primitive_proc(object_t *(*fn)(struct object_t *arguments)) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_PRIMITIVE_PROC;
	obj->data.primitive_proc.fn = fn;
	return obj;
}
	
char is_primitive_proc(object_t *obj) {
	return obj->type == T_PRIMITIVE_PROC;
}
	
object_t *is_null_proc(object_t *arguments) {
	return is_the_empty_list(car(arguments)) ? true : false;
}
	
object_t *is_boolean_proc(object_t *arguments) {
	return is_boolean(car(arguments)) ? true : false;
}
	
object_t *is_symbol_proc(object_t *arguments) {
	return is_symbol(car(arguments)) ? true : false;
}

object_t *is_integer_proc(object_t *arguments) {
	return is_fixnum(car(arguments)) ? true : false;
}
object_t *is_float_proc(object_t *arguments) {
	return is_float(car(arguments)) ? true : false;
}
object_t *is_number_proc(object_t *arguments) {
	return is_number(car(arguments)) ? true : false;
}

object_t *is_char_proc(object_t *arguments) {
	return is_character(car(arguments)) ? true : false;
}

object_t *is_string_proc(object_t *arguments) {
	return is_string(car(arguments)) ? true : false;
}

object_t *is_pair_proc(object_t *arguments) {
	return is_pair(car(arguments)) ? true : false;
}

object_t *is_procedure_proc(object_t *arguments) {
	object_t *obj;
	
	obj = car(arguments);
	return (is_primitive_proc(obj) ||
	is_compound_proc(obj)) ?
	true :
	false;
}

object_t *char_to_integer_proc(object_t *arguments) {
	return make_fixnum((car(arguments))->data.character.value);
}
object_t *char_to_float_proc(object_t *arguments) {
	return make_float((car(arguments))->data.character.value);
}
object_t *char_to_number_proc(object_t *arguments) {
	return make_fixnum((car(arguments))->data.character.value);
}
object_t *integer_to_char_proc(object_t *arguments) {
	return make_character((car(arguments))->data.fixnum.value);
}
object_t *float_to_char_proc(object_t *arguments) {
	return make_character((car(arguments))->data.dotted.value);
}
object_t *number_to_char_proc(object_t *arguments) {
	if(is_float(car(arguments))){
		return float_to_char_proc(arguments);
	}
	return integer_to_char_proc(arguments);
}
object_t *number_to_string_proc(object_t *arguments) {
	char buffer[100];
	if(is_float(car(arguments))){
		sprintf(buffer, "%f", (car(arguments))->data.dotted.value);
	}
	else{
		sprintf(buffer, "%ld", (car(arguments))->data.fixnum.value);
	}
	return make_string(buffer);
}
object_t *string_to_number_proc(object_t *arguments) {
	if(is_float(car(arguments))){
		return make_float(atof((car(arguments))->data.string.value));
	}
	return make_fixnum(atoi((car(arguments))->data.string.value));
}

object_t *symbol_to_string_proc(object_t *arguments) {
	return make_string((car(arguments))->data.symbol.value);
}

object_t *string_to_symbol_proc(object_t *arguments) {
	return make_symbol((car(arguments))->data.string.value);
}
char are_number_args(object_t *arguments){
	object_t *ptr;
	int argno=1;
	ptr=arguments;
	while(!is_the_empty_list(ptr)){
		if(ptr->type!=T_INTEGER || ptr->type!= T_FLOAT) return argno;
		ptr=cdr(ptr);
		argno++;
	}
	return 0;
}
char are_float_args(object_t *arguments){
	object_t *ptr;
	int argno=1;
	ptr=arguments;
	while(!is_the_empty_list(ptr)){
		if(car(ptr)->type== T_FLOAT) return 1;
		ptr=cdr(ptr);
		argno++;
	}
	return 0;
}

object_t *add_proc(object_t *arguments) {
	long result = 0;
	float fresult=0;
	
	if(are_float_args(arguments)){
		while (!is_the_empty_list(arguments)) {
			if(is_float(car(arguments))){
				fresult += (car(arguments))->data.dotted.value;
			}
			else{
				fresult += (float)(car(arguments))->data.fixnum.value;
			}
			arguments = cdr(arguments);
		}
		return make_float(fresult);
	}
	else{
		while (!is_the_empty_list(arguments)) {
			result += (car(arguments))->data.fixnum.value;
			arguments = cdr(arguments);
		}
		return make_fixnum(result);
	}
}

object_t *sub_proc(object_t *arguments) {
	long result;
	float fresult=0;
	
	if(are_float_args(arguments)){
		if(is_float(car(arguments))){
			fresult = (car(arguments))->data.dotted.value;
		}
		else{
			fresult = (float)(car(arguments))->data.fixnum.value;
		}
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if(is_float(car(arguments))){
				fresult -= (car(arguments))->data.dotted.value;
			}
			else{
				fresult -= (float)(car(arguments))->data.fixnum.value;
			}
			//arguments = cdr(arguments);
		}
		return make_float(fresult);
	}
	else{
		result = (car(arguments))->data.fixnum.value;
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			result -= (car(arguments))->data.fixnum.value;
			//arguments = cdr(arguments);
		}
		return make_fixnum(result);
	}
}

object_t *mul_proc(object_t *arguments) {
	long result = 1;
	float fresult=1;
	
	if(are_float_args(arguments)){
		while (!is_the_empty_list(arguments)) {
			if(is_float(car(arguments))){
				fresult *= (car(arguments))->data.dotted.value;
			}
			else{
				fresult *= (float)(car(arguments))->data.fixnum.value;
			}
			arguments = cdr(arguments);
		}
		return make_float(fresult);
	}
	else{
		while (!is_the_empty_list(arguments)) {
			result *= (car(arguments))->data.fixnum.value;
			arguments = cdr(arguments);
		}
		return make_fixnum(result);
	}
}

object_t *quotient_proc(object_t *arguments) {
	//float f1,f2;
	if(are_float_args(arguments)){
// 		if(is_float(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
// 		if(is_float(cadr(arguments))) {f2=((car(arguments))->data.dotted.value);} else {f2=(float)((car(arguments))->data.fixnum.value);}
// 		return make_float(
// 			f1/
// 			f2);
		fprintf(stderr,"float arguments not allowed.\n");
		return bottom;
	}
	else{
		return make_fixnum(
			((car(arguments) )->data.fixnum.value)/
			((cadr(arguments))->data.fixnum.value));
	}
}

object_t *remainder_proc(object_t *arguments) {
	//float f1,f2;
	if(are_float_args(arguments)){
// 		if(is_float(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
// 		if(is_float(cadr(arguments))) {f2=((car(arguments))->data.dotted.value);} else {f2=(float)((car(arguments))->data.fixnum.value);}
// 		return make_float(
// //			f1% // % don't get float operands
// 			f2);
		fprintf(stderr,"float arguments not allowed.\n");
		return bottom;
	}
	else{
		return make_fixnum(
			((car(arguments) )->data.fixnum.value)%
			((cadr(arguments))->data.fixnum.value));
	}
}

object_t *is_number_equal_proc(object_t *arguments) {
	long value;
	float f1,f2;
	if(are_float_args(arguments)){
		if(is_float(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if(is_float(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 != f2) {
				return false;
			}
		}
		return true;
	}
	else{
		value = (car(arguments))->data.fixnum.value;
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if (value != ((car(arguments))->data.fixnum.value)) {
				return false;
			}
		}
		return true;
	}
}

object_t *is_less_than_proc(object_t *arguments) {
	long previous;
	long next;
	float f1,f2;
	
	if(are_float_args(arguments)){
		if(is_float(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if(is_float(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 < f2) {
				f1=f2;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		previous = (car(arguments))->data.fixnum.value;
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			next = (car(arguments))->data.fixnum.value;
			if (previous < next) {
				previous = next;
			}
			else {
				return false;
			}
		}
		return true;
	}
}

object_t *is_greater_than_proc(object_t *arguments) {
	long previous;
	long next;
	float f1,f2;
	
	if(are_float_args(arguments)){
		if(is_float(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if(is_float(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 > f2) {
				f1=f2;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		previous = (car(arguments))->data.fixnum.value;
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			next = (car(arguments))->data.fixnum.value;
			if (previous > next) {
				previous = next;
			}
			else {
				return false;
			}
		}
		return true;
	}
}

object_t *cons_proc(object_t *arguments) {
	return cons(car(arguments), cadr(arguments));
}

object_t *car_proc(object_t *arguments) {
	return caar(arguments);
}

object_t *cdr_proc(object_t *arguments) {
	return cdar(arguments);
}

object_t *set_car_proc(object_t *arguments) {
	set_car(car(arguments), cadr(arguments));
	return ok_symbol;
}

object_t *set_cdr_proc(object_t *arguments) {
	set_cdr(car(arguments), cadr(arguments));
	return ok_symbol;
}

object_t *list_proc(object_t *arguments) {
	return arguments;
}

object_t *is_eq_proc(object_t *arguments) {
	object_t *obj1;
	object_t *obj2;
	
	obj1 = car(arguments);
	obj2 = cadr(arguments);
	
	if (obj1->type != obj2->type) {
		return false;
	}
	switch (obj1->type) {
		case T_INTEGER:
			return (obj1->data.fixnum.value == 
			obj2->data.fixnum.value) ?
			true : false;
			break;
		case T_FLOAT:
			return (obj1->data.dotted.value == 
			obj2->data.dotted.value) ?
			true : false;
			break;
		case T_CHARACTER:
			return (obj1->data.character.value == 
			obj2->data.character.value) ?
			true : false;
			break;
		case T_STRING:
			return (strcmp(obj1->data.string.value, 
						   obj2->data.string.value) == 0) ?
						   true : false;
						   break;
		default:
			return (obj1 == obj2) ? true : false;
	}
}

object_t *apply_proc(object_t *arguments) {
	fprintf(stderr, "illegal state: The body of the apply "
	"primitive procedure should not execute.\n");
	exit(1);
}

object_t *interaction_environment_proc(object_t *arguments) {
	return the_global_environment;
}

object_t *null_environment_proc(object_t *arguments) {
	return setup_environment();
}

object_t *environment_proc(object_t *arguments) {
	return make_environment();
}

object_t *eval_proc(object_t *arguments) {
	fprintf(stderr, "illegal state: The body of the eval "
	"primitive procedure should not execute.\n");
	exit(1);
}

object_t *load_proc(object_t *arguments) {
	char *filename;
	//FILE *in;
	object_t *exp;
	object_t *result;
	stream_t stream;
	
	filename = car(arguments)->data.string.value;
	//in = fopen(filename, "r");
	init_stream(&stream);
	open_stream(&stream,filename,TSTREAM_FILE);
	if (stream.fp == NULL) {
		fprintf(stderr, "could not load file \"%s\"", filename);
		//exit(1);
		return bottom;
	}
	while ((exp = read_sx(&stream)) != eof_object) {
		result = eval(exp, the_global_environment);
	}
	close_stream(&stream);
	return result;
}

object_t *open_input_port_proc(object_t *arguments) {
	char *filename;
	FILE *in;
	
	filename = car(arguments)->data.string.value;
	in = fopen(filename, "r");
	if (in == NULL) {
		fprintf(stderr, "could not open file \"%s\"\n", filename);
		//exit(1);
		return bottom;
	}
	return make_input_port(in);
}

object_t *close_input_port_proc(object_t *arguments) {
	int result;
	
	result = fclose(car(arguments)->data.input_port.stream);
	if (result == EOF) {
		fprintf(stderr, "could not close input port\n");
		//exit(1);
		return bottom;
	}
	return ok_symbol;
}

object_t *is_input_port_proc(object_t *arguments) {
	return is_input_port(car(arguments)) ? true : false;
}

object_t *read_proc(object_t *arguments) {
	//FILE *in;
	object_t *result;
	stream_t stream;
	
	init_stream(&stream);
	stream.fp = is_the_empty_list(arguments) ?
	stdin :
	car(arguments)->data.input_port.stream;
	open_stream(&stream,NULL,TSTREAM_STDIN);
	result = read_sx(&stream);
	return (result == NULL) ? eof_object : result;
}

object_t *read_char_proc(object_t *arguments) {
	FILE *in;
	int result;
	
	in = is_the_empty_list(arguments) ?
	stdin :
	car(arguments)->data.input_port.stream;
	result = getc(in);
	return (result == EOF) ? eof_object : make_character(result);
}

object_t *peek_char_proc(object_t *arguments) {
	FILE *in;
	int result;
	
	in = is_the_empty_list(arguments) ?
	stdin :
	car(arguments)->data.input_port.stream;
	result = peek_(in);
	return (result == EOF) ? eof_object : make_character(result);
}

object_t *is_eof_object_proc(object_t *arguments) {
	return is_eof_object(car(arguments)) ? true : false;
}

object_t *open_output_port_proc(object_t *arguments) {
	char *filename;
	FILE *out;
	
	filename = car(arguments)->data.string.value;
	out = fopen(filename, "w");
	if (out == NULL) {
		fprintf(stderr, "could not open file \"%s\"\n", filename);
		//exit(1);
		return bottom;
	}
	return make_output_port(out);
}

object_t *close_output_port_proc(object_t *arguments) {
	int result;
	
	result = fclose(car(arguments)->data.output_port.stream);
	if (result == EOF) {
		fprintf(stderr, "could not close output port\n");
		//exit(1);
		return bottom;
	}
	return ok_symbol;
}

object_t *is_output_port_proc(object_t *arguments) {
	return is_output_port(car(arguments)) ? true : false;
}

object_t *write_char_proc(object_t *arguments) {
	object_t *character;
	FILE *out;
	
	character = car(arguments);
	arguments = cdr(arguments);
	out = is_the_empty_list(arguments) ?
	stdout :
	car(arguments)->data.output_port.stream;
	putc(character->data.character.value, out);    
	fflush(out);
	return ok_symbol;
}

object_t *write_proc(object_t *arguments) {
	object_t *exp;
	FILE *out;
	
	exp = car(arguments);
	arguments = cdr(arguments);
	out = is_the_empty_list(arguments) ?
	stdout :
	car(arguments)->data.output_port.stream;
	write_sx(out, exp);
	fflush(out);
	return ok_symbol;
}

object_t *error_proc(object_t *arguments) {
	while (!is_the_empty_list(arguments)) {
		write_sx(stderr, car(arguments));
		fprintf(stderr, " ");
		arguments = cdr(arguments);
	};
	printf("\nexiting\n");
	exit(1);
}

object_t *make_compound_proc(object_t *parameters, object_t *body, object_t *env) {
	object_t *obj;
	
	obj = alloc_object();
	obj->type = T_COMPOUND_PROC;
	obj->data.compound_proc.parameters = parameters;
	obj->data.compound_proc.body = body;
	obj->data.compound_proc.env = env;
	return obj;
}
   
char is_compound_proc(object_t *obj) {
	return obj->type == T_COMPOUND_PROC;
}
   
object_t *make_input_port(FILE *stream) {
	object_t *obj;
   
	obj = alloc_object();
	obj->type = T_INPUT_PORT;
	obj->data.input_port.stream = stream;
	return obj;
}

char is_input_port(object_t *obj) {
	return obj->type == T_INPUT_PORT;
}
   
object_t *make_output_port(FILE *stream) {
	object_t *obj;
   
	obj = alloc_object();
	obj->type = T_OUTPUT_PORT;
	obj->data.output_port.stream = stream;
	return obj;
}
   
char is_output_port(object_t *obj) {
	return obj->type == T_OUTPUT_PORT;
}
   
char is_eof_object(object_t *obj) {
	return obj == eof_object;
}

object_t *enclosing_environment(object_t *env) {
	return cdr(env);
}

object_t *first_frame(object_t *env) {
	return car(env);
}

object_t *make_frame(object_t *variables, object_t *values) {
	return cons(variables, values);
}

object_t *frame_variables(object_t *frame) {
	return car(frame);
}

object_t *frame_values(object_t *frame) {
	return cdr(frame);
}

void add_binding_to_frame(object_t *var, object_t *val, object_t *frame) {
	set_car(frame, cons(var, car(frame)));
	set_cdr(frame, cons(val, cdr(frame)));
}

object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env) {
	return cons(make_frame(vars, vals), base_env);
}
 
object_t *lookup_variable_value(object_t *var, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!is_the_empty_list(vars)) {
			if (var == car(vars)) {
				return car(vals);
			}
			if(var==cdr(vars)){
				//printf("found &rest param \n");//,cdr(vars)->data.string.value);
				return cdr(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
			if(!vars) break;
			if(vars->type!=T_PAIR) break;
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "unbound variable, %s\n", var->data.symbol.value);
	//exit(1);
	return bottom;
}
 
void set_variable_value(object_t *var, object_t *val, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;
 
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!is_the_empty_list(vars)) {
			if (var == car(vars)) {
				set_car(vals, val);
				return;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "unbound variable, %s\n", var->data.symbol.value);
	//exit(1);
}
 
void define_variable(object_t *var, object_t *val, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;

	frame = first_frame(env);    
	vars = frame_variables(frame);
	vals = frame_values(frame);

	while (!is_the_empty_list(vars)) {
		if (var == car(vars)) {
			set_car(vals, val);
			return;
		}
		vars = cdr(vars);
		vals = cdr(vals);
	}
	add_binding_to_frame(var, val, frame);
}
 
object_t *setup_environment(void) {
	object_t *initial_env;
 
	initial_env = extend_environment(
	the_empty_list,
	the_empty_list,
	the_empty_environment);
	return initial_env;
}

void populate_environment(object_t *env) {
 
	#define add_procedure(scheme_name, c_name)              \
	define_variable(make_symbol(scheme_name),           \
	make_primitive_proc(c_name),        \
	env);
 
	add_procedure("null?"      , is_null_proc);
	add_procedure("boolean?"   , is_boolean_proc);
	add_procedure("symbol?"    , is_symbol_proc);
	add_procedure("integer?"   , is_integer_proc);
	add_procedure("char?"      , is_char_proc);
	add_procedure("string?"    , is_string_proc);
	add_procedure("pair?"      , is_pair_proc);
	add_procedure("procedure?" , is_procedure_proc);
 
	add_procedure("char->integer" , char_to_integer_proc);
	add_procedure("integer->char" , integer_to_char_proc);
	add_procedure("number->string", number_to_string_proc);
	add_procedure("string->number", string_to_number_proc);
	add_procedure("symbol->string", symbol_to_string_proc);
	add_procedure("string->symbol", string_to_symbol_proc);
 
	add_procedure("+"        , add_proc);
	add_procedure("-"        , sub_proc);
	add_procedure("*"        , mul_proc);
	add_procedure("quotient" , quotient_proc);
	add_procedure("remainder", remainder_proc);
	add_procedure("="        , is_number_equal_proc);
	add_procedure("<"        , is_less_than_proc);
	add_procedure(">"        , is_greater_than_proc);
 
	add_procedure("cons"    , cons_proc);
	add_procedure("car"     , car_proc);
	add_procedure("cdr"     , cdr_proc);
	add_procedure("set-car!", set_car_proc);
	add_procedure("set-cdr!", set_cdr_proc);
	add_procedure("list"    , list_proc);
 
	add_procedure("eq?", is_eq_proc);
 
	add_procedure("apply", apply_proc);
 
	add_procedure("interaction-environment", interaction_environment_proc);
	add_procedure("null-environment", null_environment_proc);
	add_procedure("environment"     , environment_proc);
	add_procedure("eval"            , eval_proc);
 
	add_procedure("load"             , load_proc);
	add_procedure("open-input-port"  , open_input_port_proc);
	add_procedure("close-input-port" , close_input_port_proc);
	add_procedure("input-port?"      , is_input_port_proc);
	add_procedure("read"             , read_proc);
	add_procedure("read-char"        , read_char_proc);
	add_procedure("peek-char"        , peek_char_proc);
	add_procedure("eof-object?"      , is_eof_object_proc);
	add_procedure("open-output-port" , open_output_port_proc);
	add_procedure("close-output-port", close_output_port_proc);
	add_procedure("output-port?"     , is_output_port_proc);
	add_procedure("write-char"       , write_char_proc);
	add_procedure("write"            , write_proc);
 
	add_procedure("error", error_proc);
}
 
object_t *make_environment(void) {
	object_t *env;
 
	env = setup_environment();
	populate_environment(env);
	return env;
}
 

void init_env(void) {
	the_empty_list = alloc_object();
	the_empty_list->type = T_EMPTY_LIST;
	
	false = alloc_object();
	false->type = T_BOOLEAN;
	false->data.boolean.value = 0;
	
	true = alloc_object();
	true->type = T_BOOLEAN;
	true->data.boolean.value = 1;
	
	bottom=alloc_object();
	bottom->type=T_BOTTOM;
	
	symbol_table = the_empty_list;
	quote_symbol = make_symbol("quote");
	define_symbol = make_symbol("define");
	set_symbol = make_symbol("set!");
	ok_symbol = make_symbol("ok");
	if_symbol = make_symbol("if");
	lambda_symbol = make_symbol("lambda");
	begin_symbol = make_symbol("begin");
	cond_symbol = make_symbol("cond");
	else_symbol = make_symbol("else");
	let_symbol = make_symbol("let");
	and_symbol = make_symbol("and");
	or_symbol = make_symbol("or");
	quasiquote_symbol=make_symbol("quasiquote");
	unquote_symbol=make_symbol("unquote");
	unquote_splicing_symbol=make_symbol("unquote-splicing");
	
	eof_object = alloc_object();
	eof_object->type = T_EOF_OBJECT;
	
	the_empty_environment = the_empty_list;
	
	the_global_environment = make_environment();
}
