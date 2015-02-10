#include<stdio.h>
#include<stdlib.h>
#include "object.h"

object_t* alloc_object(){
	object_t *obj;
	
	obj=malloc(sizeof(object_t));
	if(obj==NULL){
		fprintf(stderr,"out of memory.\n");
		exit(1);
	}
	return obj;
}
char is_the_empty_list(object_t *obj) {
	return obj == the_empty_list;
}

char is_boolean(object_t *obj) {
	return obj->type == T_BOOLEAN;
}

char is_false(object_t *obj) {
	return obj == false;
}

char is_true(object_t *obj) {
	return !is_false(obj);
}
char is_bottom(object_t *obj){
	return obj->type==T_BOTTOM;
}
char is_number(object_t *obj){
	return (obj->type==T_INTEGER || obj->type==T_FLOAT);
}
