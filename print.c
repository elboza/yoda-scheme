#include<stdio.h>
#include "object.h"
#include "env.h"
#include "print.h"

void write_pair(FILE *out, object_t *pair) {
	object_t *car_obj;
	object_t *cdr_obj;
	
	car_obj = car(pair);
	cdr_obj = cdr(pair);
	write_sx(out, car_obj);
	if (cdr_obj->type == T_PAIR) {
		fprintf(out, " ");
		write_pair(out, cdr_obj);
	}
	else if (cdr_obj->type == T_EMPTY_LIST) {
		return;
	}
	else {
		fprintf(out, " . ");
		write_sx(out, cdr_obj);
	}
}

void write_sx(FILE *out, object_t *obj) {
	char c;
	char *str;
	
	if(!out) out=stdout;
	switch (obj->type) {
		case T_EMPTY_LIST:
			fprintf(out, "()");
			break;
		case T_BOOLEAN:
			fprintf(out, "#%c", is_false(obj) ? 'f' : 't');
			break;
		case T_BOTTOM:
			fprintf(out,"_|_");
			break;
		case T_SYMBOL:
			fprintf(out, "%s", obj->data.symbol.value);
			break;
		case T_INTEGER:
			fprintf(out, "%ld", obj->data.fixnum.value);
			break;
		case T_FLOAT:
			fprintf(out, "%g", obj->data.dotted.value);
			break;
		case T_CHARACTER:
			c = obj->data.character.value;
			fprintf(out, "#\\");
			switch (c) {
				case '\n':
					fprintf(out, "newline");
					break;
				case ' ':
					fprintf(out, "space");
					break;
				default:
					putc(c, out);
			}
			break;
		case T_STRING:
			str = obj->data.string.value;
			fputc('"',out);
			while (*str != '\0') {
				switch (*str) {
					case '\n':
						fprintf(out, "\\n");
						break;
					case '\\':
						fprintf(out, "\\\\");
						break;
					case '"':
						fprintf(out, "\\\"");
						break;
					default:
						putc(*str, out);
				}
				str++;
			}
			fputc('"',out);
			break;
		case T_PAIR:
			fprintf(out, "(");
			write_pair(out, obj);
			fprintf(out, ")");
			break;
		case T_PRIMITIVE_PROC:
			fprintf(out, "#<primitive-procedure>");
			break;
		case T_COMPOUND_PROC:
			fprintf(out, "#<compound-procedure>");
			break;
		case T_INPUT_PORT:
			fprintf(out, "#<input-port>");
			break;
		case T_OUTPUT_PORT:
			fprintf(out, "#<output-port>");
			break;
		case T_EOF_OBJECT:
			fprintf(out, "#<eof>");
			break;
		case T_WARN_MSG:
			fprintf(out,"%s\n",obj->data.string.value);
			break;
		default:
			fprintf(stderr, "cannot write unknown type\n");
			//exit(1);
	}
}

void write_debug(FILE *out, object_t *obj) {
	char c;
	char *str;
	
	if(!out) out=stdout;
	if(!obj){ fprintf(out,"NIL"); return;}
	switch (obj->type) {
		case T_EMPTY_LIST:
			fprintf(out, "()");
			break;
		case T_BOOLEAN:
			fprintf(out, "#%c", is_false(obj) ? 'f' : 't');
			break;
		case T_BOTTOM:
			fprintf(out,"_|_");
			break;
		case T_SYMBOL:
			fprintf(out, "%s", obj->data.symbol.value);
			break;
		case T_INTEGER:
			fprintf(out, "%ld", obj->data.fixnum.value);
			break;
		case T_FLOAT:
			fprintf(out, "%g", obj->data.dotted.value);
			break;
		case T_CHARACTER:
			c = obj->data.character.value;
			fprintf(out, "#\\");
			switch (c) {
				case '\n':
					fprintf(out, "newline");
					break;
				case ' ':
					fprintf(out, "space");
					break;
				default:
					putc(c, out);
			}
			break;
		case T_STRING:
			str = obj->data.string.value;
			fputc('"',out);
			while (*str != '\0') {
				switch (*str) {
					case '\n':
						fprintf(out, "\\n");
						break;
					case '\\':
						fprintf(out, "\\\\");
						break;
					case '"':
						fprintf(out, "\\\"");
						break;
					default:
						putc(*str, out);
				}
				str++;
			}
			fputc('"',out);
			break;
		case T_PAIR:
			fprintf(out, "(cons ");
			write_debug(out, car(obj));
			fprintf (out," ");
			write_debug(out, cdr(obj));
			fprintf(out, " )");
			break;
		case T_PRIMITIVE_PROC:
			fprintf(out, "#<primitive-procedure>");
			break;
		case T_COMPOUND_PROC:
			fprintf(out, "#<compound-procedure>");
			break;
		case T_INPUT_PORT:
			fprintf(out, "#<input-port>");
			break;
		case T_OUTPUT_PORT:
			fprintf(out, "#<output-port>");
			break;
		case T_EOF_OBJECT:
			fprintf(out, "#<eof>");
			break;
		case T_WARN_MSG:
			fprintf(out,"%s\n",obj->data.string.value);
			break;
		default:
			fprintf(stderr, "cannot write unknown type\n");
			//exit(1);
	}
}

void display_pair(FILE *out, object_t *pair) {
	object_t *car_obj;
	object_t *cdr_obj;
	
	car_obj = car(pair);
	cdr_obj = cdr(pair);
	display_sx(out, car_obj);
	if (cdr_obj->type == T_PAIR) {
		fprintf(out, " ");
		display_pair(out, cdr_obj);
	}
	else if (cdr_obj->type == T_EMPTY_LIST) {
		return;
	}
	else {
		fprintf(out, " . ");
		display_sx(out, cdr_obj);
	}
}

void display_sx(FILE *out, object_t *obj) {
	char c;
	char *str;
	
	if(!out) out=stdout;
	switch (obj->type) {
		case T_EMPTY_LIST:
			fprintf(out, "()");
			break;
		case T_BOOLEAN:
			fprintf(out, "#%c", is_false(obj) ? 'f' : 't');
			break;
		case T_BOTTOM:
			fprintf(out,"_|_");
			break;
		case T_SYMBOL:
			fprintf(out, "%s", obj->data.symbol.value);
			break;
		case T_INTEGER:
			fprintf(out, "%ld", obj->data.fixnum.value);
			break;
		case T_FLOAT:
			fprintf(out, "%g", obj->data.dotted.value);
			break;
		case T_CHARACTER:
			c = obj->data.character.value;
			fprintf(out, "#\\");
			switch (c) {
				case '\n':
					//fprintf(out, "newline");
					//break;
				case ' ':
					//fprintf(out, "space");
					//break;
				default:
					putc(c, out);
			}
			break;
		case T_STRING:
			str = obj->data.string.value;
			//fputc('"',out);
			while (*str != '\0') {
				switch (*str) {
					case '\n':
						//fprintf(out, "\\n");
						//break;
					case '\\':
						//fprintf(out, "\\\\");
						//break;
					case '"':
						//fprintf(out, "\\\"");
						//break;
					default:
						putc(*str, out);
				}
				str++;
			}
			//fputc('"',out);
			break;
		case T_PAIR:
			fprintf(out, "(");
			write_pair(out, obj);
			fprintf(out, ")");
			break;
		case T_PRIMITIVE_PROC:
			fprintf(out, "#<primitive-procedure>");
			break;
		case T_COMPOUND_PROC:
			fprintf(out, "#<compound-procedure>");
			break;
		case T_INPUT_PORT:
			fprintf(out, "#<input-port>");
			break;
		case T_OUTPUT_PORT:
			fprintf(out, "#<output-port>");
			break;
		case T_EOF_OBJECT:
			fprintf(out, "#<eof>");
			break;
		case T_WARN_MSG:
			fprintf(out,"%s\n",obj->data.string.value);
			break;
		default:
			fprintf(stderr, "cannot write unknown type\n");
			//exit(1);
	}
}