#ifndef READ_H
#define READ_H

char is_delimiter(int c);
char is_initial(int c);
int peek_(FILE *in);
void eat_whitespace(stream_t *stream);
void eat_expected_string(stream_t *stream, char *str);
void peek_expected_delimiter(stream_t *stream);
object_t *read_character(stream_t *stream);
object_t *read_pair(stream_t *stream);
object_t *read(stream_t *stream);

#endif