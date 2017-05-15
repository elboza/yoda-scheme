#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"
#include "macro.h"

int is_define_macro(object_t *obj){
	if(is_symbol(car(obj)) && (strcmp("defmacro",car(obj)->data.symbol.value)==0)){
		return 1;
	}
	return 0;
}
object_t* is_macro(object_t *obj){
	object_t *x;
	if(obj==NULL) return NULL;
	if(obj==the_empty_list) return NULL;
	if((x=lookup_macro(obj,the_macro_environment))!=NULL){
		printf("trovato !!\n");
		return x;
	}
	return NULL;
}
object_t* define_macro(object_t *obj){
	printf("macro %s defined\n",caadr(obj)->data.symbol.value);
	object_t *name= caadr(obj);
	object_t *args= cdadr(obj);
	object_t *body= cddr(obj);
	object_t *this_macro=make_compound_proc(args,body,the_empty_list);
	define_variable(name, 
                       this_macro,
                       the_macro_environment);
	//delete_object(obj);
	return bottom;
}
object_t* expand_macro(object_t *obj,object_t *vals){
	//delete_object(obj);
	return bottom;
}
object_t* macro(object_t *obj){
	object_t *x;
	if(is_define_macro(obj)) return define_macro(obj);
	if((x=is_macro(obj))!=NULL) return expand_macro(x,cdr(obj));
	return obj;
}
object_t *lookup_macro(object_t *var, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!is_the_empty_list(vars)) {
			if (car(var) == car(vars)) {
				return car(vals);
			}
			if(car(var)==cdr(vars)){
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
	//fprintf(stderr, "unbound variable, %s\n", var->data.symbol.value);
	//exit(1);
	return NULL;
}