#include<stdio.h>
#include<string.h>
#include "main.h"
#include "stream.h"

void init_stream(stream_t *s){
	s->filename=NULL;
	s->len=0;
	s->fp=NULL;
	s->ptr=NULL;
	s->start=NULL;
	s->type=TSTREAM_NULL;
}
void open_stream(stream_t *s,char *p,stream_type t){
	s->type=t;
	switch(t){
		case TSTREAM_FILE:
			s->filename=p;
			if((s->fp=fopen(p,"r"))==0){
				printf("error opening file (%s)\n",p);
				return;
			}
			fseek(s->fp,0L,SEEK_END);
			s->len=ftell(s->fp);
			fseek(s->fp,0L,SEEK_SET);
			break;
		case TSTREAM_STR:
			s->start=p;
			s->ptr=p;
			s->len=strlen(p);
			break;
		case TSTREAM_STDIN:
			s->fp=stdin;
			break;
		default:
			printf("unknown stream type.\n");
			init_stream(s);
			break;
	}

}
void close_stream(stream_t *s){
	if(s->type==TSTREAM_FILE) {
		if(s->fp){
			fclose(s->fp);
		}
	}
	init_stream(s);
}
int stream_get_ch_(stream_t *s){
	int c;
	switch(s->type){
		case TSTREAM_FILE:
		case TSTREAM_STDIN:
			c=(char)fgetc(s->fp);
			break;
		case TSTREAM_STR:
			if(s->ptr>=s->start+s->len) return EOF;
			c=*(s->ptr++);
			break;
		default:
			c='\0';
			break;
	}
	return c;
}
void stream_unget_ch(stream_t *s,int c){
	switch(s->type){
		case TSTREAM_FILE:
		case TSTREAM_STDIN:
			ungetc(c,s->fp);
			break;
		case TSTREAM_STR:
			if(s->ptr<=s->start) return;
			if(c == EOF) return;
			*(--s->ptr)=c;
			break;
		default:

			break;
	}
}
int stream_peek_ch(stream_t *s){
	int c;
	c=stream_get_ch_(s);
	stream_unget_ch(s,c);
	return c;
}
