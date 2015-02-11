/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
// #include <editline/readline.h>
// #include <editline/history.h>
#include "main.h"
#include "config.h"
#ifdef HAVE_LIBREADLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif
#include "object.h"
#include "stream.h"
#include "read.h"
#include "env.h"
#include "eval.h"
#include "print.h"
#include "repl.h"


extern char *strtok_r(char *, const char *, char **);
//for linux u can use __strtok_r
extern char *strdup(const char *s);

// char* rl_gets(char *s){
// 	char* input = readline(s);
// 	add_history(input);
// 	return input;
// }
char *ltrim(char *s)
{
	while(isspace(*s)) s++;
	return s;
}

char *rtrim(char *s)
{
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

char *trim(char *s)
{
	return rtrim(ltrim(s));
}
#ifdef HAVE_LIBREADLINE
char* rl_gets (char *prompt)
{
	static char *line_read = (char *)NULL;
	/* If the buffer has already been allocated,
	 *    return the memory to the free pool. */
	if (line_read)
	{
		free (line_read);
		line_read = (char *)NULL;
	}
	
	/* Get a line from the user. */
	line_read = (char *)readline (prompt);
	
	/* If the line has any text in it,
	 *    save it on the history. */
	if (line_read && *line_read)
		add_history (line_read);
	
	return (line_read);
}
#endif
void repl(stream_t *stream)
{
	char *cmd,prompt[255];
	quit_shell=0;
	int line_no=1;
	//if(check_funny()) funny_shell_disclaimer(); else normal_shell_disclaimer();
	//printf("?!?entering shell-interactive mode...\n");
	#ifdef HAVE_LIBREADLINE
	rl_variable_bind("blink-match-paren","on");
	#endif
	while(!quit_shell)
	{
		sprintf(prompt,"scm~%d~ ",line_no++);
		#ifdef HAVE_LIBREADLINE
		cmd=rl_gets(prompt);
		#else
		printf("%s",prompt);
		cmd=(char*)malloc(MAX_CMD);
		fgets(cmd,MAX_CMD,stdin);
		cmd[strlen(cmd) - 1] = '\0';
		#endif
		execute(stream,cmd);
		#ifndef HAVE_LIBREADLINE
		if(cmd) free(cmd);
		#endif
	}
}
void show_help(){
	printf("scheme interpreter, by Fernando Iazeolla 2015(c)\n");
	printf(":q                ~ exit interpreter\n");
	printf(":h                ~ this help\n");
	printf(":l file           ~ load file\n");
}
void execute(stream_t *stream,char *s)
{
	char *ns=NULL;
	object_t *exp;
	
	if(!s) return;
	if((strcmp(s,":q"))==0) {quit_shell=1;return;}
	if((strcmp(s,":h"))==0) {show_help();return;}
	strtok_r(s," ",&ns);
	//if((strcmp(s,":l"))==0){if(ns){run(trim(ns),given_env,ret,*reversefuck);}}
	printf("== %s\n",s);
	//openstream and execute lisp prog.
	open_stream(stream,s,TSTREAM_STR);
	while (!is_eof_object(exp = read(stream))) {
//		exp = read(stream);
		write(NULL, eval(exp, the_global_environment));
	}
	close_stream(stream);
}
