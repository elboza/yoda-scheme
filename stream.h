#ifndef STREAM_H
#define STREAM_H

typedef enum {TSTREAM_NULL,TSTREAM_FILE,TSTREAM_STR,TSTREAM_STDIN} stream_type;
typedef struct m_stream{
	char *filename;
	FILE *fp;
	char *ptr,*start;
	long len;
	stream_type type;
} stream_t;

void init_stream(stream_t *s);
void open_stream(stream_t *s,char *p,stream_type t);
void close_stream(stream_t *s);
char stream_get_ch(stream_t *s);
void stream_unget_ch(stream_t *s,char c);
char stream_peek_ch(stream_t *s);


#endif
