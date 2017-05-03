#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"
#include "macro.h"

object_t* is_define_macro(object_t *obj){
	if(is_symbol(car(obj)) && (strcmp("defmacro",car(obj)->data.symbol.value)==0)){
		printf("is macro...\n");
	}
	return 0;
}
int is_macro(object_t *obj){
	return 0;
}
object_t* define_macro(object_t *obj){
	return NULL;
}
object_t* expand_macro(object_t *obj){
	return NULL;
}
object_t* macro(object_t *obj){
	if(is_define_macro(obj)) return define_macro(obj);
	if(is_macro(obj)) return expand_macro(obj);
	return obj;
}