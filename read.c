#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"


char is_delimiter(int c) {
	return isspace(c) || c == EOF ||
	c == '('   || c == ')' ||
	c == '"'   || c == ';';
}

char is_initial(int c) {
	return isalpha(c) || c == '*' || c == '/' || c == '>' ||
	c == '<' || c == '=' || c == '?' || c == '!';
}

int peek_(FILE *in) {
	int c;
	
	c = getc(in);
	ungetc(c, in);
	return c;
}

void eat_whitespace(stream_t *stream) {
	int c;
	
	while ((c = stream_get_ch_(stream)) != EOF) {
		if (isspace(c)) {
			continue;
		}
		else if (c == ';') { /* comments are whitespace also */
			while (((c = stream_get_ch_(stream)) != EOF) && (c != '\n'));
			continue;
		}
		stream_unget_ch(stream,c);
		break;
	}
}

void eat_expected_string(stream_t *stream, char *str) {
	int c;
	
	while (*str != '\0') {
		c = stream_get_ch_(stream);
		if (c != *str) {
			fprintf(stderr, "unexpected character '%c'\n", c);
			//exit(1);
			return;
		}
		str++;
	}
}

void peek_expected_delimiter(stream_t *stream) {
	if (!is_delimiter(stream_peek_ch(stream))) {
		fprintf(stderr, "character not followed by delimiter\n");
		//exit(1);
		return;
	}
}

object_t *read_character(stream_t *stream) {
	int c;
	
	c = stream_get_ch_(stream);
	switch (c) {
		case EOF:
			fprintf(stderr, "incomplete character literal\n");
			//exit(1);
			return bottom;
		case 's':
			if (stream_peek_ch(stream) == 'p') {
				eat_expected_string(stream, "pace");
				peek_expected_delimiter(stream);
				return make_character(' ');
			}
			break;
		case 'n':
			if (stream_peek_ch(stream) == 'e') {
				eat_expected_string(stream, "ewline");
				peek_expected_delimiter(stream);
				return make_character('\n');
			}
			break;
	}
	peek_expected_delimiter(stream);
	return make_character(c);
}

object_t *read_pair(stream_t *stream) {
	int c;
	object_t *car_obj;
	object_t *cdr_obj;
	
	eat_whitespace(stream);
	
	c = stream_get_ch_(stream);
	if (c == ')') { /* read the empty list */
		return the_empty_list;
	}
	stream_unget_ch(stream,c);
	
	car_obj = read(stream);
	
	eat_whitespace(stream);
	
	c = stream_get_ch_(stream);    
	if (c == '.') { /* read improper list */
		c = stream_peek_ch(stream);
		if (!is_delimiter(c)) {
			fprintf(stderr, "dot not followed by delimiter\n");
			//exit(1);
			return bottom;
		}
		cdr_obj = read(stream);
		eat_whitespace(stream);
		c = stream_get_ch_(stream);
		if (c != ')') {
			fprintf(stderr,"where was the trailing right paren?\n");
			//exit(1);
			return bottom;
		}
		return cons(car_obj, cdr_obj);
	}
	else { /* read list */
		stream_unget_ch(stream,c);
		cdr_obj = read_pair(stream);        
		return cons(car_obj, cdr_obj);
	}
}

object_t *read(stream_t *stream) {
	int c;
	short sign = 1;
	int i;
	long num = 0;
	#define BUFFER_MAX 1000
	char buffer[BUFFER_MAX];
	
	eat_whitespace(stream);
	
	c = stream_get_ch_(stream);    
	
	if (c == '#') { /* read a boolean or character */
		c = stream_get_ch_(stream);
		switch (c) {
			case 't':
				return true;
			case 'f':
				return false;
			case '\\':
				return read_character(stream);
			default:
				fprintf(stderr,"unknown boolean or character literal\n");
				//exit(1);
				return bottom;
		}
	}
	else if (isdigit(c) || (c == '-' && (isdigit(stream_peek_ch(stream))))) {
		/* read a fixnum */
		if (c == '-') {
			sign = -1;
		}
		else {
			stream_unget_ch(stream,c);
		}
		while (isdigit(c = stream_get_ch_(stream))) {
			num = (num * 10) + (c - '0');
		}
		num *= sign;
		if (is_delimiter(c)) {
			stream_unget_ch(stream,c);
			return make_fixnum(num);
		}
		else {
			fprintf(stderr, "number not followed by delimiter\n");
			//exit(1);
			return bottom;
		}
	}
	else if (is_initial(c) ||
		((c == '+' || c == '-') &&
		is_delimiter(stream_peek_ch(stream)))) { /* read a symbol */
			i = 0;
			while (is_initial(c) || isdigit(c) ||
				c == '+' || c == '-') {
				/* subtract 1 to save space for '\0' terminator */
				if (i < BUFFER_MAX - 1) {
					buffer[i++] = c;
				}
				else {
					fprintf(stderr, "symbol too long. "
					"Maximum length is %d\n", BUFFER_MAX);
					//exit(1);
					return bottom;
				}
				c = stream_get_ch_(stream);
				}
				if (is_delimiter(c)) {
					buffer[i] = '\0';
					stream_unget_ch(stream,c);
					return make_symbol(buffer);
				}
				else {
					fprintf(stderr, "symbol not followed by delimiter. "
					"Found '%c'\n", c);
					//exit(1);
					return bottom;
				}
		}
		else if (c == '"') { /* read a string */
			i = 0;
			while ((c = stream_get_ch_(stream)) != '"') {
				if (c == '\\') {
					c = stream_get_ch_(stream);
					if (c == 'n') {
						c = '\n';
					}
				}
				if (c == EOF) {
					fprintf(stderr, "non-terminated string literal\n");
					//exit(1);
					return bottom;
				}
				/* subtract 1 to save space for '\0' terminator */
				if (i < BUFFER_MAX - 1) {
					buffer[i++] = c;
				}
				else {
					fprintf(stderr, 
							"string too long. Maximum length is %d\n",
			 BUFFER_MAX);
					//exit(1);
					return bottom;
				}
			}
			buffer[i] = '\0';
			return make_string(buffer);
		}
		else if (c == '(') { /* read the empty list or pair */
			return read_pair(stream);
		}
		else if (c == '\'') { /* read quoted expression */
			return cons(quote_symbol, cons(read(stream), the_empty_list));
		}
		else if (c == EOF) {
			return eof_object;
		}
		else {
			fprintf(stderr, "bad input. Unexpected '%c'\n", c);
			//exit(1);
			return bottom;
		}
		fprintf(stderr, "read illegal state\n");
		//exit(1);
		return bottom;
}
