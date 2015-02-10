#ifndef REPL_H
#define REPL_H

void repl(stream_t *stream);
int quit_shell;
#define MAX_CMD 1024
char* rl_gets (char *prompt);
void execute(stream_t *stream,char *s);

#endif
