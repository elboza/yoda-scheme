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
int is_macro(object_t *obj){
	object_t *x;
	if(obj==NULL) return 0;
	if(obj==the_empty_list) return 0;
	if((x=lookup_variable_value(obj,the_macro_environment))!=NULL){
		printf("trovato !!\n");
	}
	return 0;
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
object_t* expand_macro(object_t *obj){
	//delete_object(obj);
	return bottom;
}
object_t* macro(object_t *obj){
	if(is_define_macro(obj)) return define_macro(obj);
	if(is_macro(obj)) return expand_macro(obj);
	return obj;
}